
% this is the KNN version that interactively outputs a prediction result
% under the specified K value and number of test sample
clear
clc

% import data set
load zoo;
[n,d]=size(zoo);
class_num=length(unique(zoo(:,d)));

% min-max normalization
for i=1:d-1
    zoo(:,i)=(zoo(:,i)-min(zoo(:,i)))/(max(zoo(:,i))-min(zoo(:,i)));
end

% form train and text set
train_list=randperm(101,70);
label_train=zoo(train_list,d);
x_train=zoo(train_list,1:d-1);
test_list=setdiff((1:101),train_list);
label_test=zoo(test_list,d);
x_test=zoo(test_list,1:d-1);

% input hyper parameter K
fprint1='Please input the value of K and press "Enter"\n';
K=input(fprint1);

% input a sample for prediction
fprint2='Please input the number (within 31) of a test sample for prediction and press "Enter"\n ';
p=input(fprint2);
xp=x_test(p,:);
dist_list=dist(xp,x_train');
neighbor=zeros(1,K);
pred_count=zeros(1,class_num);
for i=1:K
    [~,neighbor(i)]=min(dist_list);
    dist_list(neighbor(i))=inf;
    pred_count(label_train(neighbor(i)))=pred_count(label_train(neighbor(i)))+1;
end
[~,pred_class]=max(pred_count);

disp('Predicted class is:')
disp(pred_class)
if pred_class==label_test(p)
    disp('The prediction is CORRECT !')
else
    disp('The prediction is INCORRECT lol')
end
