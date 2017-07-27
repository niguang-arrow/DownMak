import torch
import torch.nn as nn
import os
from os.path import join
from torch.autograd import Variable
import torch.optim as optim
import torch.backends.cudnn as cudnn
from torch.utils.data import DataLoader
from data import DatasetFromHdf5
from model import VDSR


opt = type("Option", (object,), {})

opt.seed = 123
opt.nEpoch = 50
opt.batchSize = 128
opt.threads = 1
opt.learningRate = 0.1
opt.step = 10
opt.momentum = 0.9
opt.weight_decay = 1e-4
opt.clip = 0.4


def adjust_learning_rate(epoch):
    lr = opt.learningRate * (0.1 ** (epoch // opt.step))
    for param_group in optimizer.param_groups:
        param_group['lr'] = lr


def train(epoch):

    adjust_learning_rate(epoch - 1)

    net.train()
    for iteration, batch in enumerate(training_data_loader, 1):
        input, target = Variable(batch[0].cuda()), Variable(batch[1].cuda())
        optimizer.zero_grad()
        output = net(input)
        loss = criterion(output, target)
        loss.backward()
        nn.utils.clip_grad_norm(net.parameters(), opt.clip)
        optimizer.step()

        if iteration % 100 == 0:
            print ">>> Epoch[%d](%d/%d), Loss: %.5f" % (epoch, iteration, len(training_data_loader), loss.data[0])


def save_checkpoint(epoch):
    checkpoint = 'model/'
    if not os.path.exists(checkpoint):
        os.makedirs(checkpoint)
    torch.save(net, join(checkpoint, 'model_epoch_{}.pth'.format(epoch)))
    print "Checkpoint saved to {}".format('model_epoch_{}'.format(epoch))


if __name__ == '__main__':
    torch.manual_seed(opt.seed)
    torch.cuda.manual_seed(opt.seed)

    # net = nn.DataParallel(VDSR()).cuda()
    net = nn.DataParallel(VDSR()).cuda()
    criterion = nn.MSELoss(size_average=False).cuda()

    # optimizer = optim.Adam(net.parameters(), lr=opt.learningRate)
    optimizer = optim.SGD(net.parameters(),
                        lr=opt.learningRate,
                        momentum=opt.momentum,
                        weight_decay=opt.weight_decay)

    cudnn.benchmark = True

    print("===> Loading datasets")
    train_set = DatasetFromHdf5("../vdsr/data/train.h5")
    training_data_loader = DataLoader(dataset=train_set,
                                    num_workers=opt.threads,
                                    batch_size=opt.batchSize,
                                    shuffle=True)
    print len(training_data_loader)

    for epoch in range(1, opt.nEpoch + 1):
        train(epoch)
        save_checkpoint(epoch)
