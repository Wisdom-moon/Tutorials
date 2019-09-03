#This implement a fully-connected ReLU netwrok, it has a single hidden layer.
import torch

class TwoLayerNet(torch.nn.Module):
	def __init__(self, D_in, H, D_out):
		super(TwoLayerNet, self).__init__()
		self.linear1 = torch.nn.Linear(D_in, H)
		self.linear2 = torch.nn.Linear(H, D_out)

	def forward(self, x):
		h_relu = self.linear1(x).clamp(min=0)
		y_pred = self.linear2(h_relu)
		return y_pred

dtype = torch.float
device = torch.device("cpu")

#N is batch size; D_in is input dimension;
#H is hiddden dimension; D_out is output dimension.
N,D_in,H,D_out = 64,1000,100,10

#Create random input and output data
x = torch.randn(N, D_in, device=device, dtype=dtype)
y = torch.randn(N, D_out, device=device, dtype=dtype)

#nn.Sequential is a Module which contains other Modules, and applies them in
#sequence to product its output.
#Each Linear Module computes output from input using a linear function,
#and holds internal Tensors for its weight and bias.
model = TwoLayerNet(D_in, H, D_out)


loss_fn = torch.nn.MSELoss(reduction='sum')

learning_rate = 1e-4
optimizer = torch.optim.SGD(model.parameters(), lr=learning_rate)
for t in range(500):
	#Forward pass: compute predicted y
	y_pred = model(x)

	#compute and print loss
	loss = loss_fn(y_pred, y)
	print (t, loss.item())

	#Backprop to compute gradients of w1 and w2 with respect to loss
	optimizer.zero_grad()
	loss.backward()

	#Update weights
	optimizer.step()
