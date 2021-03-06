func main()
{
	Int[] a;
	const Int[] b;
	const Int blength;

	Int i = 0;
	Int j = 0;
	while(i < blength)
	{
		a[i] = b[j];
		i = i + 1;
		j = j + 1;
	}
}

(axiom
	(forall ((it Nat))
		(= (i (l9 it)) (j (l9 it)))
	)
)

(conjecture
	(forall ((k Int))
		(=>
			(and
				(<= 0 blength)
				(<= 0 k)
				(< k blength)
			)
			(= (a main_end k) (b k))
		)
	)
)
