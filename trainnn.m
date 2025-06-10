% Define the number of images
numImages = 20; % Since the images are numbered from 0 to 63824

% Preallocate the cell array to hold the images
images = cell(numImages, 1);
angles = zeros(numImages, 1); % Replace this with actual angles if available
data = readtable('data2.txt', 'Format', '%s %f', 'Delimiter', ' ');
% Extract filenames and angles from the table
fileNames = data.Var1;
anglesTemp = data.Var2;
% Read each image and store it in the cell array
for i = 0:numImages-1
    filename = sprintf('%d.jpg', i); % Assuming the images are in JPEG format
    images{i+1} = imread(filename);
     angles(i+1) = anglesTemp(i+1);
end
imagesAll = zeros(256,455, 3, numImages); % Adjust the size according to your images
for i = 1:numImages
     imTemp= images{i};
     imagesAll(:, :, :, i) =imTemp;
end
% Combine the images into a single dataset (if needed)
% [combinedImages, combinedAngles] = yourCombineFunction(images, angles);

% Split the dataset into training, validation, and test sets
[trainInd, valInd, testInd] = dividerand(numImages, 0.7, 0.15, 0.15);

trainImages = images(trainInd);
valImages = images(valInd);
testImages = images(testInd);

trainAngles = angles(trainInd);
valAngles = angles(valInd);
testAngles = angles(testInd);

% Now you can use trainImages, valImages, and testImages in the previous CNN code
% Load your image dataset and preprocess it
% [images, angles] = yourDataLoadingFunction();

% Define the layers of the CNN
layers = [
    imageInputLayer([256 455 3], 'Name', 'input', 'Normalization', 'none')
    convolution2dLayer(3, 8, 'Padding', 'same', 'Name', 'conv_1')
    batchNormalizationLayer('Name', 'batchnorm_1')
    reluLayer('Name', 'relu_1')
    maxPooling2dLayer(2, 'Stride', 2, 'Name', 'maxpool_1')
    convolution2dLayer(3, 16, 'Padding', 'same', 'Name', 'conv_2')
    batchNormalizationLayer('Name', 'batchnorm_2')
    reluLayer('Name', 'relu_2')
    maxPooling2dLayer(2, 'Stride', 2, 'Name', 'maxpool_2')
    dropoutLayer(0.5, 'Name', 'dropout')
    fullyConnectedLayer(64, 'Name', 'fc')
    reluLayer('Name', 'relu_3')
    fullyConnectedLayer(1, 'Name', 'fc2')
    regressionLayer('Name', 'output')
];

% Set the options for training
options = trainingOptions('adam', ...
    'MaxEpochs', 10, ...
    'InitialLearnRate', 1e-4, ...
    'Verbose', false, ...
    'Plots', 'training-progress');
numImagesTrain = size(trainAngles,1);
imagesAllTrain = zeros(256,455, 3, numImagesTrain); % Adjust the size according to your images
for i = 1:numImagesTrain
     imTemp= trainImages{i};
     imagesAllTrain(:, :, :, i) =imTemp;
end

numImagesTest = size(testAngles,1);
imagesAllTest = zeros(256,455, 3, numImagesTest); % Adjust the size according to your images
for i = 1:numImagesTest
     imTemp= testImages{i};
     imagesAllTest(:, :, :, i) =imTemp;
end


% Train the network
 net = trainNetwork(imagesAllTrain, trainAngles, layers, options);

% To predict angles, use the trained network
predictedAngles = predict(net, imagesAllTest);
disp(predictedAngles -testAngles);