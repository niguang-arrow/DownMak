from torchvision.transforms import Compose, CenterCrop, ToTensor, Scale, Normalize
from torchvision.transforms import RandomCrop, RandomHorizontalFlip
from dataset import DatasetFromFolder
# import numpy as np

# np.random.seed(456)

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

def common_transform(crop_size):
    return Compose([
        RandomCrop(crop_size),
        RandomHorizontalFlip(),
    ])

def input_transform(crop_size, upscale_factor):
    """
    @functions:
        input transform
    """
    return Compose([
        Scale(crop_size // upscale_factor),
        Scale(crop_size),
        ToTensor(),
        Normalize((0,), (1,)),
    ])

def target_transform(crop_size):
    """
    @functions:
        target transform
    """
    return Compose([
        # CenterCrop(crop_size),
        ToTensor(),
        Normalize((0,), (1,)),
    ])

def get_training_set(train_dir, upscale_factor):
    """
    @functions:
        return training set for DataLoader
    @parameters:
        train_dir: i.e. ./Train/
        upscale_factor: i.e. 3
    """
    crop_size = calculate_valid_crop_size(33, upscale_factor)

    return DatasetFromFolder(train_dir,
                            common_transform = common_transform(crop_size),
                            input_transform = input_transform(crop_size, upscale_factor),
                            target_transform = target_transform(crop_size))

def get_testing_set(test_dir, upscale_factor):
    """
    @functions:
        return testing set for DataLoader
    @parameters:
        test_dir: i.e. ./Test/Set5
        upscale_factor: i.e. 3
    """
    crop_size = calculate_valid_crop_size(228, upscale_factor)

    return DatasetFromFolder(test_dir,
                            common_transform = common_transform(crop_size),
                            input_transform = input_transform(crop_size, upscale_factor),
                            target_transform = target_transform(crop_size))


if __name__ == '__main__':
    from PIL import Image
    from torchvision.transforms import ToPILImage
    import matplotlib.pyplot as plt
    figname = './Test/Set5/butterfly_GT.bmp'
    im = Image.open(figname).convert('L')
    input = common_transform(90)(im)
    target = input.copy()
    input = input_transform(90, 3)(input)
    target = target_transform(90)(target)
    img = ToPILImage()(input)
    tar = ToPILImage()(target)
    # img.show()
    plt.figure()
    plt.subplot(121)
    plt.imshow(img)
    plt.subplot(122)
    plt.imshow(tar)
    plt.show()
