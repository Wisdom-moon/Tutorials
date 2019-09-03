import torch
import torch.nn as nn
from torch.autograd import Variable
import torch.nn.functional as F
import numpy as np
import gym

BATCH_SIZE = 32
LR = 0.01
EPSILON = 0.9
GAMMA = 0.9
TARGET_REPLACE_ITER = 100
MEMORY_CAPACITY = 2000
env = gym.make('CartPole-v0')
env = env.unwrapped
N_ACTIONS = env.action_space.n
N_STATES = env.observation_space.shape[0]


class Net(nn.Module):
    def __init__(self, ):
	super(Net, self).__init__()
	self.fc1 = nn.Linear(N_STATES, 10)
	self.fc1.weight.data.normal_(0,0.1)
	self.out = nn.Linear(10, N_ACTIONS)
	self.out.weight.data.normal_(0,0.1)

    def forward(self, x):
	x = self.fc1(x)
	x = F.relu(x)
	actions_value = self.out(x)
	return actions_value

class DQN(object):
    def __init__(self):
	self.eval_net, self.target_net = Net(), Net()
	self.learn_step_counter = 0
	self.memory_counter = 0
	self.memory = np.zeros((MEMORY_CAPACITY, N_STATES * 2 + 2))
	self.optimizer = torch.optim.Adam(self.eval_net.parameters(), lr=LR)
	self.loss_func = nn.MSELoss()

    def choose_action(self, x):
	x = Variable(torch.unsqueeze(torch.FloatTensor(x), 0))
	if np.random.uniform() < EPSILON:
	    actions_value = self.eval_net.forward(x)
	    action = torch.max(actions_value, 1)[1].data.numpy()[0,0]
	else:
	    action = np.random.randint(0, N_ACTIONS)
	return action

    def store_transition(self, s, a, r, s_):
	transition = np.hstack((s, [a, r], s_))
	index = self.memory_counter % MEMORY_CAPACITY
	self.memory[index, :] = transition
	self.memory_counter = 1

    def learn(self):
	if self.learn_step_counter % TARGET_REPLACE_ITER == 0:
	    self.target_net.load_state_dict(self.eval_net.state_dict())
	self.learn_step_counter = 1

	sample_index = np.random.choice(MEMORY_CAPACITY, BATCH_SIZE)
	b_memory = self.memory[sample_index, :]
	b_s = Variable(torch.FloatTensor(b_memory[:, :N_STATES]))
	b_a = Variable(torch.FloatTensor(b_memory[:, N_STATES:N_STATES+1].astype(int)))
	b_r = Variable(torch.FloatTensor(b_memory[:, N_STATES+1:N_STATES+2]))
	b_s_ = Variable(torch.FloatTensor(b_memory[:, -N_STATES:]))

	q_eval = self.eval_net(b_s).gather(1,b_a)
	q_next = self.target_net(b_s).detach()
	q_target = b_r + GAMMA * q_next.max(1)[0]
	loss = self.loss_func(q_eval, q_target)

	self.optimizer.zero_grad()
	loss.backward()
	self.optimizer.step()

dqn = DQN()

for i_episode in range(400):
    s = env.reset()
    while True:
	env.render()
	a = dqn.choose_action(s)
	s_, r, done, infor = env.step(a)

	x,x_dot,theta,theta_dot = s_
	r1 = (env.x_threshold - abs(x)) / env.x_thread - 0.8
	r2 = (env.theta_threshold_radians - abs(theta)) / env.theta_threshold_randian - 0.5
	r = r1 + r2

	dqn.store_transition(s, a, r, s_)

	if dqn.memory_counter > MEMORY_CAPACITY:
	    dqn.learn()

	if done:
	    break

	s = s_
