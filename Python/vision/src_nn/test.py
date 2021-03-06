import torch
import torch.nn as nn
from torch.autograd import Variable
from torchvision.transforms import Compose
from torchvision.transforms import ToTensor, Normalize, Scale, CenterCrop, ToPILImage
from model import SRCNN
from os.path import join
from PIL import Image
import numpy as np
import matplotlib.pyplot as plt
from main import nEpochs


def load_image(filename):
    im = Image.open(filename).convert('YCbCr')
    y, Cb, Cr = im.split()
    return y, Cb, Cr


def calculate_valid_crop_size(crop_size, upscale_factor):
    """
    @functions:
        When a low resolution image upscales with factor `upscale_factor`,
        the size of the generated image may not be same as `crop_size`, for
        mod(crop_size, upscale_factor) may not be zero. So we need to calculate
        the true size of the generated image.
    @parameters:
        crop_size: crop size of input image
        upscale_factor: upscale factor
    @return:
        real size of high resolution image
    """
    return crop_size - (crop_size % upscale_factor)


def getPSNR(pred, gt, maxVal=255.):
    """
    @parameters:
        pred: Image object predicted from model
        gt: Image object ground truth
    """
    noise, clean = np.array(pred).astype(float), np.array(gt).astype(float)
    Diff = noise - clean
    rmse = np.sqrt(np.mean(Diff ** 2))
    return 20 * np.log10(maxVal / rmse)


figname = 'butterfly_GT.bmp'
# figname = 'bird_GT.bmp'
test_dir = './Test/Set5'
upscale_factor = 3

filename = join(test_dir, figname)
y, Cb, Cr = load_image(filename)
width, height = y.size
crop_size = calculate_valid_crop_size(min(width, height), upscale_factor)

gt_transforms = Compose([
    CenterCrop(crop_size),
])

bicubic_transforms = Compose([
    CenterCrop(crop_size),
    Scale(crop_size // upscale_factor),
    Scale(crop_size),
])

input_transforms = Compose([
    CenterCrop(crop_size),
    Scale(crop_size // upscale_factor),
    Scale(crop_size),
    ToTensor(),
    Normalize((0.,), (1.,)),
])

output_transforms = Compose([
    Normalize((0.,), (1.,)),
    ToPILImage(),
])

gt_y, gt_Cb, gt_Cr = (gt_transforms(im) for im in [y, Cb, Cr])
bicubic_y, bicubic_Cb, bicubic_Cr = (bicubic_transforms(im) for im in [y, Cb, Cr])

model_path = './model/model_epoch_{}.pth'.format(4000)
net = nn.DataParallel(SRCNN()).cuda()
print net
net.load_state_dict(torch.load(model_path))

input = Variable(input_transforms(y).view(1, 1, crop_size, crop_size).cuda())
pred = net(input)

pred = pred.data.cpu()
pred[pred < 0.0] = 0.0
pred[pred > 1.0] = 1.0

pred_y = output_transforms(pred.view(1, crop_size, crop_size))
# bicubic_y = output_transforms(input_transforms(y))

print type(pred_y)

print "Bicubic PSNR: ", getPSNR(bicubic_y, gt_y)
print "Predict PSNR: ", getPSNR(pred_y, gt_y)

# pred_y.show()

gt_img = Image.merge('YCbCr', (gt_y, gt_Cb, gt_Cr)).convert('RGB')
bicubic_img = Image.merge('YCbCr', (bicubic_y, bicubic_Cb, bicubic_Cr)).convert('RGB')
pred_img = Image.merge('YCbCr', (pred_y, bicubic_Cb, bicubic_Cr)).convert('RGB')

plt.figure()
plt.subplot(131)
plt.imshow(gt_img)
plt.subplot(132)
plt.imshow(bicubic_img)
plt.subplot(133)
plt.imshow(pred_img)
plt.show()
