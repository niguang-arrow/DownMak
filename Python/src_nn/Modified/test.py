import torch
import torch.nn as nn
from torch.autograd import Variable
from model import SRCNN
from os.path import join
from PIL import Image
import numpy as np
# from scipy.ndimage import imread
import scipy.io as sio
import matplotlib.pyplot as plt
from main import nEpochs
import math


def load_image(filename):
    im = Image.open(filename).convert('YCbCr')
    y, Cb, Cr = im.split()
    return y, Cb, Cr


def valid_crop_size(img, upscale_factor):
    h, w, c = img.shape
    h_shave_border = h % upscale_factor
    w_shave_border = w % upscale_factor
    return img[h_shave_border : h,
               w_shave_border : w, :]


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


def PSNR(pred, gt, shave_border=0):
    height, width = pred.shape[:2]
    pred = pred[shave_border:height - shave_border, shave_border:width - shave_border]
    gt = gt[shave_border:height - shave_border, shave_border:width - shave_border]
    imdff = pred - gt
    rmse = math.sqrt(np.mean(imdff ** 2))
    if rmse == 0:
        return 100
    return 20 * math.log10(255.0 / rmse)


def colorize(y, ycbcr):
    img = np.zeros((y.shape[0], y.shape[1], 3), np.uint8)
    img[:, :, 0] = y
    img[:, :, 1] = ycbcr[:, :, 1]
    img[:, :, 2] = ycbcr[:, :, 2]
    img = Image.fromarray(img, "YCbCr").convert("RGB")
    return img


figname = 'butterfly_GT'
ext = '.bmp'
# figname = 'bird_GT.bmp'
test_dir = 'Set5/'
upscale_factor = 3

# gt_name = join(test_dir, figname + ext)
# scale_name = test_dir + figname + "_scale_" + str(upscale_factor) + ext

# im_gt_ycbcr = imread(gt_name, mode="YCbCr")
# im_gt_ycbcr = valid_crop_size(im_gt_ycbcr, upscale_factor)
# im_b_ycbcr = imread(scale_name, mode="YCbCr")

# print im_gt_ycbcr.shape
# print im_b_ycbcr.shape

im_gt_y = sio.loadmat("Set5/" + figname + ".mat")['im_gt_y']
im_b_y = sio.loadmat("Set5/" + figname + ".mat")['im_b_y']

im_gt_y = im_gt_y.astype(float)
im_b_y = im_b_y.astype(float)

# psnr_bicubic = getPSNR(im_b_y, im_gt_y)
psnr_bicubic = PSNR(im_b_y, im_gt_y, 4)

im_input = im_b_y / 255.
im_input = Variable(torch.from_numpy(im_input).float()).view(1, -1, im_input.shape[0], im_input.shape[1])

model_path = './model/model_epoch_{}.pth'.format(4000)
net = nn.DataParallel(SRCNN()).cuda()
print net
net.load_state_dict(torch.load(model_path))

pred = net(im_input)

pred = pred.data.cpu().numpy()
pred[pred < 0.0] = 0.0
pred[pred > 1.0] = 1.0
pred_y = pred * 255

print "Bicubic PSNR: ", psnr_bicubic
print "Predict PSNR: ", getPSNR(pred_y, im_gt_y)

print pred_y[0].transpose(1, 2, 0).shape
# im_h = colorize(pred_y[0, :, :], im_b_ycbcr)
# im_gt = Image.fromarray(im_gt_ycbcr, "YCbCr").convert("RGB")
# im_b = Image.fromarray(im_b_ycbcr, "YCbCr").convert("RGB")

# pred_y.show()
plt.figure()
plt.subplot(131)
plt.imshow(im_gt_y, cmap='gray')
plt.subplot(132)
plt.imshow(im_b_y, cmap='gray')
plt.subplot(133)
plt.imshow(pred_y[0].squeeze(), cmap='gray')
plt.show()
