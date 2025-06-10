%in the name of Allah
numImages = 208;

% Read the table containing filenames and angles
data = readtable('data2.txt', 'Format', '%s %f', 'Delimiter', ' ');
% Initialize the arrays to hold images and angles
imageData = zeros(256, 455, 3, numImages); % Adjust the size according to your images
labelData = zeros(numImages, 1);
% Read each image and store it in the array
for i = 1:numImages
    filename = sprintf('%d.jpg', i); % Adjust the format if needed
    imageData(:, :, :, i) = imread(filename);
    labelData(i) = data.Var2(i);
end

idx = randperm(numImages);
numTrain = round(0.8 * numImages);
trainIdx = idx(1:numTrain);
testIdx = idx(numTrain+1:end);

trainImages = imageData(:,:,:,trainIdx);
trainLabels = labelData(trainIdx);
testImages = imageData(:,:,:,testIdx);
testLabels = labelData(testIdx);

layers = [
    imageInputLayer([256 455 3])
    convolution2dLayer(3, 8, 'Padding', 'same')
    batchNormalizationLayer
    reluLayer
    maxPooling2dLayer(2, 'Stride', 2)  
    convolution2dLayer(3, 16, 'Padding', 'same')
    batchNormalizationLayer
    reluLayer  
    maxPooling2dLayer(2, 'Stride', 2)
    convolution2dLayer(3, 32, 'Padding', 'same')
    batchNormalizationLayer
    reluLayer
    
    fullyConnectedLayer(1)
    regressionLayer
];

% تنظیمات گزینه‌های آموزش
options = trainingOptions('adam', ...
    'MaxEpochs', 300, ...
    'MiniBatchSize', 64, ...
    'InitialLearnRate', 1e-3, ...
    'Shuffle', 'every-epoch', ...
    'ValidationData', {testImages, testLabels}, ...
    'ValidationFrequency', 20, ...
    'Verbose', false, ...
    'Plots', 'training-progress');

% آموزش مدل
net = trainNetwork(trainImages, trainLabels, layers, options);

% ارزیابی مدل
predictedLabels = predict(net, testImages);
performance = mean(abs(predictedLabels - testLabels));
disp(['Out = =', num2str(performance)]);
