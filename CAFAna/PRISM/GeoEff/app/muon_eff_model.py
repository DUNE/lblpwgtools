import torch
import numpy as np

class muonEffModel(torch.nn.Module) :
    def __init__(self) :
        super(muonEffModel, self).__init__()
        
        self._nn = torch.nn.Sequential(
            # 6 inputs going into first hidden layer with 64 nodes, ReLU activation
            torch.nn.Linear(6, 64), torch.nn.ReLU(),
            # into second hidden layer with 64 nodes, ReLU activation
            torch.nn.Linear(64,64), torch.nn.ReLU(),
            # Three outputs
            torch.nn.Linear(64,3)
            )
        
    def forward (self, x) :
        return self._nn(x)

def forward(blob, train=True) :
    with torch.set_grad_enabled(train) :
        data = torch.as_tensor(blob.data).type(torch.FloatTensor).cuda()
        prediction = blob.net(data)
        
        # Training
        los, acc = -1, -1
        if blob.label is not None :
            label = torch.as_tensor(blob.label).type(torch.LongTensor).cuda()
            loss = blob.criterion(prediction, label)
        blob.loss = loss
        
        return {'prediction' : prediction.cpu().detach().numpy(),
                'loss' : loss.cpu().detach().item()}

def backward(blob) :
    blob.optimizer.zero_grad()
    blob.loss.backward()
    blob.optimizer.step()

def FillLabel(blob, data) :
    labelTemp = np.zeros((data[1].shape[0], ))

    for i in range(0, len(data[1])) :
        
        if data[1][i][0] == 1 :
            labelTemp[i] = 0
        elif data[1][i][1] == 1 :
            labelTemp[i] = 1
        else :
            labelTemp[i] = 2
    blob.label = labelTemp

def FillData(blob, data) :
    blob.data = data[0]
