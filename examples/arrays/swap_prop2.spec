func main()
{
	Int[] a;
	Int[] b;
	const Int[] olda;
	const Int[] oldb;
	const Int alength;
	const Int blength;

	Int i = 0;
	while(i < alength)
	{
		a[i] = b[i];
		b[i] = a[i];
		i = i + 1;
	}
}

(assert-not
	(forall ((j Int)(k Int))
		(=>
			(and
				(<= 0 j)
				(< j blength)
				(<= 0 k)
				(< k alength)
				(= alength blength)
				(= (a (l11 zero) k) (olda k))
				(= (b (l11 zero) k) (oldb k))
			)
			(= (b main_end j) (olda j))
		)
	)
)