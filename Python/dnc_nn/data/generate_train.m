clear;close all;

%% folder 指向的是经过 data_augmentation 处理后的数据集, 比如 291-aug, 此时可以先对 Train400 做 aug 处理
folder = './test';

savepath = 'train.h5';
patchsize = 40;
stride = 10;

%% sigma 
sigmas = [25];

%% scale factors
scales = [1, 0.9, 0.8, 0.7];

%% initialization
data = zeros(patchsize, patchsize, 1, 1);
label = zeros(patchsize, patchsize, 1, 1);

count = 0;

%% generate data
ext               =  {'*.jpg','*.png','*.bmp','*.jpeg'};
filepaths           =  [];

for i = 1 : length(ext)
    filepaths = cat(1,filepaths, dir(fullfile(folder, ext{i})));
end

for sigma = sigmas
    for i = 1 : length(filepaths)
        for s = 1 : length(scales)
            if mod(i, 100) == 0
                disp([i, length(filepaths)]);
            end
            image = imread(fullfile(folder, filepaths(i).name)); % uint8
            %[~, name, exte] = fileparts(filepaths(i).name);
            if size(image, 3) == 3
                image = rgb2gray(image);
            end
            image_label = im2double(imresize(image, scales(s), 'bicubic'));
            [hei,wid] = size(image_label);
            noise = sigma/255. * randn(size(image_label));
            image_input = image_label + noise;

            for x = 1 : stride : (hei-patchsize+1)
                for y = 1 : stride : (wid-patchsize+1)
                    count       = count+1;
                    data(:, :, 1, count)   = image_input(x : x+patchsize-1, y : y+patchsize-1,:);
                    label(:, :, 1, count)   = image_label(x : x+patchsize-1, y : y+patchsize-1,:);
                end
            end
        end
    end
end


order = randperm(count);
data = data(:, :, 1, order);
label = label(:, :, 1, order); 

%% writing to HDF5
chunksz = 64;
created_flag = false;
totalct = 0;

for batchno = 1:floor(count/chunksz)
    batchno
    last_read=(batchno-1)*chunksz;
    batchdata = data(:,:,1,last_read+1:last_read+chunksz); 
    batchlabs = label(:,:,1,last_read+1:last_read+chunksz);

    startloc = struct('dat',[1,1,1,totalct+1], 'lab', [1,1,1,totalct+1]);
    curr_dat_sz = store2hdf5(savepath, batchdata, batchlabs, ~created_flag, startloc, chunksz); 
    created_flag = true;
    totalct = curr_dat_sz(end);
end

h5disp(savepath);
