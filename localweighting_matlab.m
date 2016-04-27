function G = localweighting(W)
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

	% Create the degree centralities by explicitly summing over columns the W matrix
	degreeCentralities = sum(W')';
	C=zeros(size(W));
	n=length(W);
	for i=1:n
		for j=1:n
			s=0;
			Wi=0.0;
			Wj=0.0;
			for k=1:n
				delta_ik = (W(i,k)~=0);
				delta_jk = (W(j,k)~=0);
				
				s = s+ (delta_ik*delta_jk*(W(i,k)+W(j,k))/2);
				Wi = Wi+W(i,k);
				Wj = Wj+W(j,k);
			end
			C(i,j) = C(i,j) + 2*( W(i,j)+s)/(sum(W(:,i))+sum(W(:,j)));
		end
	end

	Wtilde = Wbar.*C;
	G = Wtilde;
	for i=1:n
		G(i,:) = G(i,:)./sum(Wtilde(i,:));
	end