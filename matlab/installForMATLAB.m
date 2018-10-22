%   Installs AutomationShield MATLAB API.
%  
%   This code is part of the AutomationShield hardware and software
%   ecosystem. Visit http://www.automationshield.com for more
%   details. This code is licensed under a Creative Commons
%   Attribution-NonCommercial 4.0 International License.
% 
%   Created by Gergely Tak�cs. 
%   Last update: 22.10.2018.

function installForMATLAB()
    thisdir=pwd;
    addpath(genpath(thisdir));
    savepath
    disp('Automation Shield MATLAB API added to MATLAB path.')
end