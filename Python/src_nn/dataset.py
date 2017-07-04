import torch
from torch.utils.data import DataLoader, Dataset
from PIL import Image
from os import listdir
from os.path import join


def load_image(image_path):
    """
    @functions:
        load an image and convert it from RGB to YCbCr
        and return the Y channel
    @parameters:
        image_path: Image path, i.e. ./TrainSet/1.bmp
    @return:
        Y channel from YCbCr space.
    """
    im = Image.open(image_path).convert('YCbCr')
    y, _, _ = im.split()
    return y


class DatasetFromFolder(Dataset):
    """
    @functions:
        load input images and targets
    @parameters:
        image_dir: image directory, i.e. ./TrainSet
        input_transform: torchvision.transforms for input images
        target_transform: torchvision.transforms for target images
    """
    def __init__(self, image_dir, input_transform=None, target_transform=None):
        super(DatasetFromFolder, self).__init__()
        self.image_filenames = [join(image_dir, filename)
                                for filename in listdir(image_dir)]

        self.input_transform = input_transform
        self.target_transform = target_transform

    def __getitem__(self, index):
        input = load_image(self.image_filenames[index])
        target = input.copy()
        if self.input_transform:
            input = self.input_transform(input)
        if self.target_transform:
            target = self.target_transform(target)

        return input, target

    def __len__(self):
        return len(self.image_filenames)
