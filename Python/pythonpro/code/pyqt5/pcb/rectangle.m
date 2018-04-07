function statu = rectangle(path)
src=imread(path);
Ibw=im2bw(src);
[l,m]=bwlabeln(Ibw,8);
status=regionprops(l,'BoundingBox');
statu = [];
for i=1:m
    if status(i).BoundingBox(3)>60 && status(i).BoundingBox(4)>40
        statu(end + 1,:) = status(i).BoundingBox.*[1 1 2 2] - ...
                    [status(i).BoundingBox(3)/2,status(i).BoundingBox(4)/2,0,0];
        statu(end,:) = round(statu(end,:));
        %statu(end,:) = statu(end,:) + [403 452 0 0];
   end
end
end
