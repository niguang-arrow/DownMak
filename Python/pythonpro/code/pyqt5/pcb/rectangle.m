function [bbox, exact, statu] = rectangle(path)
src=imread(path);
Ibw=im2bw(src);
[l,m]=bwlabeln(Ibw,8);
status=regionprops(l,'BoundingBox');
statu = []; % 保存 ROI 中的坐标
exact = []; % 保存原图中的坐标
bbox = []; % statu 中的方框范围较大, bbox 刚好框住元器件
for i=1:m
    if status(i).BoundingBox(3)>60 && status(i).BoundingBox(4)>40
        bbox(end + 1, :) = status(i).BoundingBox + [403 452 0 0];
        statu(end + 1,:) = status(i).BoundingBox.*[1 1 2 2] - ...
                    [status(i).BoundingBox(3)/2,status(i).BoundingBox(4)/2,0,0];
        statu(end,:) = round(statu(end,:));
        %% 加上后面的数值则是求在原图中的坐标, 注释掉下一行就是求 ROI 中的坐标.
        exact(end + 1,:) = statu(end,:) + [403 452 0 0];
   end
end
end
