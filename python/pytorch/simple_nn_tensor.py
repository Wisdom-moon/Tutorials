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

#Randomly initialize weights
w1 = torch.randn(D_in, H, device=device, dtype=dtype, requires_grad=True)
w2 = torch.randn(H, D_out, device=device, dtype=dtype, requires_grad=True)

learning_rate = 1e-6
for t in range(500):
	#Forward pass: compute predicted y
	y_pred = x.mm(w1).clamp(min=0).mm(w2)

	#compute and print loss
	loss = (y_pred - y).pow(2).sum()
	print (t, loss.item())

	#Backprop to compute gradients of w1 and w2 with respect to loss
	loss.backward()

	#Update weights
	with torch.no_grad():
		w1 -= learning_rate * w1.grad
		w2 -= learning_rate * w2.grad

		w1.grad.zero_()
		w2.grad.zero_()
