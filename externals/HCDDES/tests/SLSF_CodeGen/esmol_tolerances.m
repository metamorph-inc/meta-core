function result = esmol_tolerances()
	result = {
		'product.mdl', 1e-10
        'gain.mdl', 1e-12
		'sum.mdl', 1e-13
		'trig.mdl', 1e-4
        'sqrttest.mdl', 1e-14
		'matrixinv.mdl', 1e-14
		'math.mdl', 1e-14
		'zt.mdl', 1e-11
		'discretezeropole.mdl', 1e-10
        'lookuptable1D.mdl', 1e-10
        'lookuptable2D.mdl', 1e-8
        'lookuptable3D.mdl', 1e-8
	};
end
