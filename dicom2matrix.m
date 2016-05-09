function [gray] = dicom2matrix(dir_path,num)
dlmwrite('~/Desktop/data',num,'delimiter','\t','-append');

for i=1:num
    path = strcat(dir_path,'/IM');
    path = strcat(path,num2str(i));
    I = dicomread(path);
    md = dicominfo(path);
    lop = length(md.WindowCenter);
    for j=1:lop
        center = md.WindowCenter(j)/md.RescaleSlope-md.RescaleIntercept;
        width = md.WindowWidth(j)/md.RescaleSlope-md.RescaleIntercept;
        M = mat2gray(I,[center-(width/2),center+(width/2)]);
        gray = uint8(round(M*255));
    end
    dlmwrite('~/Desktop/data',size(gray),'delimiter','\t','-append');
    dlmwrite('~/Desktop/data',gray,'delimiter','\t','-append');
end