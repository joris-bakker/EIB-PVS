% body_surface_potential
% Berechnung der Potentialverteilung in der Frontalebene
% hervorgerufen durch den Herzvektor
% in Nx x Ny Messpunkten
% Die Mitte des Dipols befindet sich im Ursprung des Koordinatensystems.
% Der Dipol liegt in der Frontalebene (xQ,yQ,0)
% Die Messpunkte liegen in der Frontalebene (xM,yM,zM), zM = konst
%________________________________________________________
% Bearbeiter: Matthias Laukner
% Datum: 12.10.23
%________________________________________________________

clear all; clf;

K = 1;           % Konstante der Lead-Field-Matrix

% Dipolstärke und Dipolkoordinaten
% (testweise ist die Dipolstärke konstant und der Dipol liegt in der
% Frontalebene mit z0 = 0)

Qp =   1;        % normierte positive Ladung
xQp =  1;        % normierte x-Koordinate der positiven Ladung
yQp =  0;        % normierte y-Koordinate der positiven Ladung

Qn =  -1;        % normierte negative Ladung
xQn = -1;        % normierte x-Koordinate der negativen Ladung
yQn =  0;        % normierte y-Koordinate der negativen Ladung

% Spezifikation der Koordinaten der Messpunkte

Nx = 64;         % Bestimmt Dimension der (Nx x Ny - Messmatrix)
Ny = 64;

xMmin = -2;     % linke Grenze der Messebene
xMmax =  2;     % rechte Grenze der Messebene

yMmin = -2;     % untere Grenze der Messebene
yMmax =  2;     % obere Grenze der Messebene

zM    = 1;      % z-Koordinate der Messebene

% Berechnung der Quellpunktvektoren

rQp = [xQp yQp 0];
rQn = [xQn yQn 0];

rQ = [rQp;rQn];    % Matrix der Quellpunktkoordinaten

% Aufbau Quellenvektor

QV = [Qp;Qn];      % Quellenvektor

% Berechnung der Messpunkte

dx = (xMmax - xMmin)/(Nx - 1);
dy = (yMmax - yMmin)/(Ny - 1);

% Messpunkte werden von unten links mit i=1 beginnend durchnummeriert
% rM(i,:) = [xMi yMi zM]

rM = zeros(Nx*Ny,3);        % Matrix der Messpunktkoordinaten initialisieren
XM = zeros(Ny,Nx);          % Matrix mit Koordinaten der x-Werte zur Bildmatrix initialisieren
YM = XM;                    % Matrix mit Koordinaten der y-Werte zur Bildmatrix initialisieren 


i = 0;
xM = xMmin;
yM = yMmin;

for k = 1:Ny               % für alle Pixel in y-Richtung
    for l = 1:Nx           % für alle Pixel in x-Richtung

        i = i + 1;         % nächster Ortsvektorindex
        rM(i,:) = [xM yM zM];
        XM(k,l) = xM;      % Matrix mit Koordinaten der x-Werte zur Bildmatrix
        YM(k,l) = yM;      % Matrix mit Koordinaten der y-Werte zur Bildmatrix

        xM = xM + dx;      % nächste x-Position
        
    end
    xM = xMmin;
    yM = yM + dy;

end

% Aufbau der Lead-Field-Matrix

Z = size(rM,1); % Anzahl Zeilen der Lead-Field-Matrix
S = size(rQ,1); % Anzahl Spalten der Lead-Field-Matrix

for z = 1:Z
    for s = 1:S
        % Kehrwert des Betrags des Abstandsvektors zwischen Messpunkt und
        % Quellpunkt berechnen:
        LM(z,s) = 1/norm((rM(z,:) - rQ(s,:)));  
        LM = K*LM; % LM skalieren
    end
end

% Potentialverteilung berechnen:

phiV = LM*QV;   % Potentialvektor = Lead-Field-Matrix*Quellenvektor
CM = zeros(Ny,Nx);  % Matrix mit Pixel-Farbwerten initialisieren

% grafische Darstellung der Potentialverteilung:

i = 1;

for k = 1:Ny               % für alle Pixel in y-Richtung
    for l = 1:Nx           % für alle Pixel in x-Richtung
        CM(k,l) = phiV(i); % Pixel-Farbwert eintragen
        i = i+1;
    end
end

%image(CM);


colormap('jet');                             % Colormap Regenbogenfarben
ih = image([xMmin xMmax],[yMmin yMmax],CM);  % Potentialverteilung als Bild ausgeben
set(ih,'CDataMapping','scaled');             % skaliertes Mapping der Farbdaten
ah = gca;
set(ah,'YDir','normal'); 
colorbar;
title('Potentialverteilung in der Frontalebene');

% Quellendipol einzeichnen
hold on;
qh = quiver(xQn,yQn,(xQp-xQn),(yQp-yQn),'off','k'); % Zeiger in Ableitungsrichtung zeichnen
set(qh,'LineWidth',2);

% Isopotentiallinien einzeichnen
[M,c] = contour(XM,YM,CM,'k'); 
c.LineWidth = 1;

axis('equal');












