Fsampl=586;	% sampling frequency
Fnyquist=Fsampl/2;	% Nyquist frequency
Fmax=20		% maximum frequency to pass
Fcut=2*Fmax;	% frequency where noise is cut to zero
N=16		% taps

% create a test function data
t = 0:2*Fsampl;				% time scale in [ms]
% elementary parts
sin2hz = sin(2*pi*t/Fsampl*2);
sin100hz = sin(2*pi*t/Fsampl*100);
sqrw100hz = (square(t/Fsampl*2*pi*100, 0.5)+1)/2;
gauss = transpose( gaussian(length(t),0.007*length(t)/Fsampl) );
noise = rand(1, length(t));

% some test functions
%y = 0.5*sin2hz;
%y = 0.5*sin2hz + 5*sin100hz;
%y = 0.5*sin2hz + 0.3*noise;
%y = 0.5*sin2hz + sqrw100hz;
%y = gauss +  sqrw100hz;
y = gauss + sqrw100hz + 0.1*noise;

% filtering
b = fir2(N, [0 Fmax/Fnyquist Fcut/Fnyquist 1], [1 0.8 0 0]);	% create filter coefficients
of = filter(b, 1, y);				% filter input function
% fixed-point version
b = round(b*256);
oi = filter(b/sum(b), 1, y);				% filter input function
% print coefficients for fixed point version
b
sum(b)

% output results on the screen
plot(t, y, 'c;input;');
hold on;
plot(t, of, 'r;output-float;');
plot(t, oi, 'b;output-int;');
hold off;