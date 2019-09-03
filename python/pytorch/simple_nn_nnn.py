#This implement a fully-connected ReLU netwrok, it has a single hidden layer.
import torch

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
model = torch.nn.Sequential(
	torch.nn.Linear(D_in, H),
	torch.nn.ReLU(),
	torch.nn.Linear(H, D_out),
	)


loss_fn = torch.nn.MSELoss(reduction='sum')

learning_rate = 1e-4
for t in range(500):
	#Forward pass: compute predicted y
	y_pred = model(x)

	#compute and print loss
	loss = loss_fn(y_pred, y)
	print (t, loss.item())

	#Backprop to compute gradients of w1 and w2 with respect to loss
	model.zero_grad()
	loss.backward()

	#Update weights
	with torch.no_grad():
		for param in model.parameters():
			param -= learning_rate * param.grad
