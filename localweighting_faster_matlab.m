function G = localweighting_faster_matlab(W)
	% ORACOLO QUESTA E' L'IMPLEMENTAZIONE CORRETTA
	% W=[0 2 2 0 1 0 0 0 0;
	% 2 0 2 2 0 0 0 0 0;
	% 2 2 0 2 0 0 0 0 1;
	% 0 2 2 0 0 0 1 0 0;
	% 1 0 0 0 0 2 0 0 2;
	% 0 0 0 0 2 0 2 2 0;
	% 0 0 0 1 0 2 0 2 0;
	% 0 0 0 0 0 2 2 0 2;
	% 0 0 1 0 2 0 0 2 0 ]

	% First normalize the weighted coupling matrix
	% Ls is the total number of links in the network
	Ls = length(nonzeros(W))/2;
	Ws = 0.5*sum(W(:));
	
	Wbar = Ls*W/Ws;
	delta = ( W(:,:)~=0 );

	% Create the degree centralities by explicitly summing over columns the W matrix
	sumW = sum(W);
	% Preallocate the C matrix
	C=zeros(size(W));
	n=length(W);
	for i=1:n
		for j=1:n
			C(i,j) = C(i,j) + 2*( W(i,j) + sum( delta(i,:).*delta(j,:).*((W(i,:)+W(j,:))*0.5) ) )/(sumW(i)+sumW(j));
		end
    end
	
    
    Wtilde = Wbar.*C;
	sumWtilde = sum(Wtilde);
	G = Wtilde;
	for i=1:n
		G(i,:) = G(i,:)./sumWtilde(i);
    end
    