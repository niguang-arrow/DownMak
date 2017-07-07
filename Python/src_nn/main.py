import torch
import torch.nn as nn
import torch.optim as optim
from torch.autograd import Variable
from torch.utils.data import DataLoader
import torch.backends.cudnn as cudnn
import numpy as np
from model import SRCNN
from data import get_training_set, get_testing_set
import os


seed = 123
upscale_factor = 3
batchSize = 30
testBatchSize = 10
nEpochs = 4000
step = 1200
learningRate = 0.001
threads = 8
train_dir = './Train'
test_dir = './Test/Set5'

torch.manual_seed(seed)
torch.cuda.manual_seed(seed)

print '>>> Loading Datasets'

train_set = get_training_set(train_dir, upscale_factor)
test_set = get_testing_set(test_dir, upscale_factor)
training_data_loader = DataLoader(dataset=train_set,
                                 num_workers=threads,
                                 batch_size=batchSize,
                                 shuffle=True)
testing_data_loader = DataLoader(dataset=test_set,
                                num_workers=threads,
                                batch_size=batchSize,
                                shuffle=False)

print '>>> Building Model'

model = nn.DataParallel(SRCNN()).cuda()
criterion = nn.MSELoss().cuda()

print model

optimizer = optim.Adam(model.parameters(), lr=learningRate)


def adjust_learning_rate(optimizer, epoch):
    lr = learningRate * (0.1 ** (epoch // step))
    for param_group in optimizer.param_groups:
        param_group['lr'] = lr


def train(epoch):
    epoch_loss = 0

    for iteration, batch in enumerate(training_data_loader, 1):
        input, target = Variable(batch[0]).cuda(), Variable(batch[1]).cuda()

        optimizer.zero_grad()
        loss = criterion(model(input), target)
        epoch_loss += loss.data[0]
        loss.backward()
        optimizer.step()

        print '>>> Epoch[{}]({}/{}): Loss: {:.4f}'.format(epoch, iteration, len(training_data_loader), loss.data[0])

    print '>>> Epoch {} Complete: Avg. Loss: {:.4f}'.format(epoch, epoch_loss / len(training_data_loader))


def test():
    avg_psnr = 0
    for batch in testing_data_loader:
        input, target = Variable(batch[0]).cuda(), Variable(batch[1]).cuda()

        prediction = model(input)
        mse = criterion(prediction, target)
        psnr = 10 * np.log10(1 / mse.data[0])
        avg_psnr += psnr

    print '>>> Avg. PSNR: {:.4f} dB'.format(avg_psnr / len(testing_data_loader))

    # # arr_size = prediction.size()
    # array = prediction[0].data.cpu()#.reshape(arr_size[2], arr_size[3])
    # im = ToPILImage()(array)
    # im.convert('L').save('result.png')


def checkpoint(epoch):
    model_path = "model/"
    if not os.path.exists(model_path):
        try:
            os.makedirs(model_path)
        except OSError:
            raise

    out_path = os.path.join(model_path, "model_epoch_{}.pth".format(epoch))
    torch.save(model.state_dict(), out_path)
    print "Checkpoint saved to {}".format(out_path)


for epoch in range(1, nEpochs + 1):
    cudnn.benchmark = True
    adjust_learning_rate(optimizer, epoch)
    train(epoch)
    test()
    if epoch == nEpochs:
        checkpoint(epoch)
