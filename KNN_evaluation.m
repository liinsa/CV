
% this is the KNN version that directly output the prediction accuracy

% clear the workspace and console
clear
clc

% import data set and initialize parameters
load zoo;
x=zoo;
k=3; % set the K value
dist_type=2; % 1: use the hamming distance; 2: use the euclidean distance
[lth,wd]=size(zoo);

% form train set and test set
train_list=randperm(lth,round(lth*0.7));
x_train=x(train_list,:);
x_train_lb=x_train(:,wd);
x_test=x;
x_test(train_list,:)=[];
x_test_lb=x_test(:,wd);

% obtain statistics of data
wd=wd-1;
[lth_test,~]=size(x_test);
[lth_train,~]=size(x_train);

% implement KNN classification for all the test samples
lb_pred=zeros(lth_test,1);
for i=1:lth_test
    dist_vec=zeros(lth_train,1);
    for j=1:lth_train
        if dist_type==1
            dist_vec(j,1)=sum(x_test(i,:)~=x_train(j,:));
        end
        if dist_type==2
            dist_vec(j,1)=sqrt(sum((x_test(i,:)-x_train(j,:)).^2));
        end
    end
    lb_nn=zeros(1,k);
    for r=1:k
        [~,winner]=min(dist_vec);
        lb_nn(1,r)=x_train_lb(winner);
        dist_vec(winner)=inf;
    end
    lb_pred(i,1)=mode(lb_nn);
end

% compute test accuracy
acc=sum(lb_pred==x_test_lb)/lth_test;

% demonstrate results
disp('The prediction accuracy is:')
disp([num2str(acc*100),'%'])

