Matlab TraceConditioning.m READ-ME

Blake Thomas

June 11, 2009

This read-me is a description of the files enclosed in this MatLabTrace.zip

Code has all of the code required to run a simulation, DataFiles has the data produced from a successful simulation, and Visualization has the code used to visualize the data as well as the figures produced by running MatLabActivity.m with the saved data.


The files in the code folder are as follows (with brief description):

CalcBoxcarExcitation.m		

calculates the excitation of one neuron by taking the scalar product of input weights with the firing matrix of the previous timestep.

CalcBoxcarInhibition.m		

calculates the shunting inhibtion due to K0 and the feedback inhibtion due to the activity of the previous timestep and the interneuron weights that can be changed by DavesRule

CalcExcitation.m	

calculates the excitation of one neuron by taking the scalar product of input weights with the firing matrix of the previous timestep. (this is only used in competitive networks) 

CalcExcitationWithInhibition.m	
	
calculates the total internal excitation of a neuron. yj = exc / (exh + inh) where exc is the scalar product of the input weights and the firing matrix of the previous timestep and inh is the inhibtion from K0, KFB, and KFF.

CalculateZbar.m

calculates the Zbar value of all the neurons. After a synapse is activated, the 'Zbar' of that synapses rises linearly to 1 in 'rise' timesteps. After it saturates at 1, Zbar declines exponentially at the rate alpha. Zbar affects how the synapses modify. 

CalculateZ0bar.m

same as CalculateZbar, but only works for the first timestep of a trial.

CompetitiveInhibition.m

competitively fires the neurons. Activity will be constant in competitive networks.
		
CreateNetwork.m

creates network. Fan_in_con is the input connections per neuron. c_in_mat are what neurons input into each neuron. w_in_mat are the  weights corresponding to the connections in c_in_mat.
		
CreateZ0.m

creates the firing pattern on the 0th timestep by randomly firing the correct number of neurons based on the desired activity level. This jump starts the network to perform normally on the first timestep. Further research is needed to determine if there should be more than one Z0 for larger boxcars, i.e. a Z00, Z01, Z02, etc.
		
DavesRule.m

modifies the weights to the feedback inter-neuron according to Dave's Rule. Lambda is the time constant for this function.

ExcitationBoxcar.m

calculates yj, the total internal 'excitation' of the neuron. yj = [exc] / ([exc] + [inh]) where [exc] is the sum of all of the saved excitations in the boxcar, [inh] is the sum of all of the saved inhibtions in the boxcar plus the current feed-forward inhibtion (KFF * sum(inputs))  

TraceConditioning.m

This is the main. This runs the simulation. All parameters are set at the beginning of this function. The end of the function contains all of the save statements. There are two main for-loops in this script. One for-loop goes across all trials, while the inner for-loop iterates through time-steps on each trial. At the end of a trial, the firing diagram is saved as training_N.mat. At the end of the simulation, a testing trial is run with out the US and this firing diagram is saved as testing_[totaltrials+1].m. After all trials are done, the two surfaces: total activity and US activity are saved, as are the final weights, K0 history, and average activity history.

UpdateWeights.m

modifies the synaptic weights based on mu, the time constant for synaptic modification, whether the neuron fired or not, and the Zbar value for the synapse.


The scripts in the Vizualization folder are as follows:

MatLabActivity.m

This is the 'main' of the visualization scripts. It creates six figures: 

figure 1 shows the activity level of the simulation over each timestep in each trial
figure 2 shows the activity level of the US neurons over each timestep in each trial
figure 3 shows average activity level in each trial
figure 4 shows the K0 value for each trial
figure 5 shows the spy diagram of the testing trial after the simulation is over
figure 6 shows the spy diagram of the last training trial

the firing file loaded in figure 5 and 6 needs to be changed to match up with the number of trials run by TraceConditioning.m

noaa_colorgradient.m

contains the color scale used for the Total Activity Surface plot. It was created by the NOAA.

reorder.m

reorders the neurons from a firing matrix in chronological order by the first time that neuron fired.

spload.m

loads a sparse matrix

SpyTraceReorderedThinned.m

converts a normal firing matrix into one ready to be shown with the spy command. 

thin.m

'thins' the reordered firing matrix to show only a fraction of the data so it will fit in a normal spy diagram


The data folder contains firing diagrams from 151 'trials' of a simulation. The total activity surface is in the file *_surface.mat, the US activity surface is in the file *_USactivit.mat, the average activity history and the K0 history are in *_Acthistory.mat and *_K0history.mat, respectively. The final weights for the simulation are stored in *_finalweights.mat. This is a HUGE file.

To see how to load these files, look at how MatLabActivity.m does it. For example, to load a firing matrix use load('training_42.mat') to load the matrix into Z (it is saved as Z and MatLab magically remembers this).


