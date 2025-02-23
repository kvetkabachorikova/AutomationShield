%   TugShield Identification example
%
%   Flex sensor feedback in the TugShield.
%
%   This example initialises and calibrates the board then set 
%   servo to 8 diferent position and watch the response of flexi
%   senzor.
%
%   This code is part of the AutomationShield hardware and software
%   ecosystem. Visit http://www.automationshield.com for more
%   details. This code is licensed under a Creative Commons
%   Attribution-NonCommercial 4.0 International License.
%
%   Created by Eva Vargov�.
%   Last update: 16.5.2021.

startScript;   

TugShield = TugShield;          % Create TugShield object from TugShield class
TugShield.begin('COM5', 'UNO'); % Initialise shield with used Port and Board type
TugShield.calibrate();          % Calibrate TugShield

Ts = 0.05;                        % Sampling period in seconds
k = 1;                            % Algorithm step counter
nextStep = 0;                     % Algorithm step flag
samplingViolation = 0;            % Sampling violation flag

R = [20, 50, 15, 90, 60, 120, 75, 180, 40, 80]; % Reference trajectory
T = 40;                                         % Section length
n = length(R);

tic                                          % Start measuring time
while (1)                                    % Infinite loop
    if (nextStep)                            % If step flag is enabled
        if (mod(k, T*i) == 1)                % At the end of section, progress in trajectory
            i = i + 1;                       
            if (i > length(R))               % If at the end of trajectory
                TugShield.actuatorWrite(0.0);
                break                        % Stop the program execution
            end
            u = R(i);                        % Progress in trajectory  
        end
        y = TugShield.sensorRead();            % Read ball position
        response(k, :) = [u, y];               % Store results
        k = k + 1;                             % Increment step counter
        nextStep = 0;                          % Disable step flag
    end                                        % Step end
    
    if (toc >= Ts * k)                            % If its time for next sample
        if (toc >= Ts * (k + 1))                  % Check if steps overlap
            disp('Sampling violation has occurred.')
            samplingViolation = 1                 % Set sampling violation flag            
            TugShield.actuatorWrite(0);             
            break                                 % Stop program if they do overlap
        end
        nextStep = 1;                             % Enable step flag
    end
end

response = response(1:k-1, :);                    % Remove unused space
save responsePID response                         % Save results in responsePID.mat file
disp('The example finished its trajectory. Results have been saved to "responsePID.mat" file.')
plotResults('responsePID.mat')                    % Plot results from responsePID.mat file


