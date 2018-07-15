function [p] = HistogramLocalization2D
%2D Localization

%1.0 Initalization of the world
dim1 = 32; dim2 = 16; 
locationindex = reshape(1:dim1*dim2,dim1,dim2)';
n = numel(locationindex);
rand('twister',5489);
world = reshape(randi([0 1],n,1),dim2,dim1); %0 = black, 1 = white
figure; imagesc(world); colormap(gray(2)); axis ij;

p = ones(dim2,dim1)*(1/n); %initialize probability

%2.0 Sequence of measurments and movements (sample case)
% [1/-1/0 1/-1/0 1/0] -> [up/down/neither right/left/neither white/black]
%measure_move = [0 1 1; 0 1 1; 0 1 0; 0 1 0; 0 1 0; 0 1 1; 0 1 1; 0 1 1; 0 1 1; 0 1 1; 0 1 0; 0 1 0; 0 1 0; 0 1 0; 0 1 1; 0 1 0; 0 1 0; 0 1 1; 0 1 1; 0 1 1;];
%measure_move = [0 1 1; 0 1 1; 0 1 0; 0 1 0; 1 0 0; 0 1 1; 0 1 1; 0 1 1; 0 1 1; 0 1 1; 0 1 0; 0 1 0; 0 1 0; 0 1 0; 0 1 1; 0 1 0; 0 1 0; 0 1 1; 0 1 1; 0 1 1;];
measure_move = [0 1 1; 0 1 1; 0 1 0; 0 1 0; 0 1 0; 0 1 1; 1 0 1; 1 0 0; 1 0 1; 0 1 0; 0 1 0; 0 1 0; 0 1 0; 0 1 1; 0 1 1; 0 1 1; 1 0 1; 1 0 0; 1 0 0; 1 0 0;];

figure;
%3.0 localization loop

    for k = 1:size(measure_move,1),
    
    %a. sensor update
    p = sense(world, p, measure_move(k,3));
    
    imagesc(p); colormap default;
    %surf(p);
    title(['step: ' num2str(k)]);
    pause;
      
    %b. movement update
    p = move(p, measure_move(k,1:2));
    
    end

end

function pnew = sense(world, p, SenVal)
  %models sensor reading
  pHit = 0.9; %default = 0.6
  pMiss = 0.1; %default = 0.2
  
  %2D multiplication step
  pnew = abs(world -(1-SenVal))*pHit + abs(world-(SenVal))*pMiss; 
  pnew = p.*pnew;
  
  %normalization
  pnew = pnew./sum(sum(pnew));

end

function pnew = move(p, MoveVal)
   %models movement error
   K = [0.1 0.1 0.1; 0.1 0.8 0.1; 0.1 0.1 0.1];
   
   %2D convolution
   pnew = conv2(p, K,'same');
   
   %update probability based on 2D movement
   md1 = mod(MoveVal(2),size(p,2));
   md2 = mod(-1*MoveVal(1),size(p,1));
   pnew = [pnew(:,end+1-md1:end) pnew(:,1:end-md1)];
   pnew = [pnew(end+1-md2:end,:); pnew(1:end-md2,:)];

   %normalization
   pnew = pnew/sum(sum(pnew));
  
end