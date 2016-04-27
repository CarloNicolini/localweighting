function G = localweighting(W,isDense,isSymmetric)

if isDense
	if isSymmetric
		G=localweighting_dense_symm_mx(W);
	else
		G=localweighting_dense_asymm_mx(W);
	end
else
	if isSymmetric
		G=localweighting_sparse_symm_mx(W);
	else
		G=localweighting_sparse_asymm_mx(W);
	end
end
