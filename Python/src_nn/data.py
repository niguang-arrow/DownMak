from torchvision.transforms import Compose, CenterCrop, ToTensor, Scale
from dataset import DatasetFromFolder

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


def input_transform(crop_size, upscale_factor):
    """
    @functions:
        input transform
    """
    return Compose([
        CenterCrop(crop_size),
        Scale(crop_size // upscale_factor),
        Scale(crop_size),
        ToTensor(),
    ])

def target_transform(crop_size):
    """
    @functions:
        target transform
    """
    return Compose([
        CenterCrop(crop_size),
        ToTensor(),
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
    crop_size = calculate_valid_crop_size(33, upscale_factor)

    return DatasetFromFolder(test_dir,
                            input_transform = input_transform(crop_size, upscale_factor),
                            target_transform = target_transform(crop_size))
