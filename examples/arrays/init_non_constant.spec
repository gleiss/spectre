func main()
{
	Int[] a;
	const Int alength;
	const Int v;

	Int i = 0;
	while(i < alength)
	{
		a[i] = (2 * i) + v;
		i = i+1;
	}
}

(assert-not
   (forall ((pos Int))
      (=>
         (and
            (<= 0 pos)
            (< pos alength)
            (<= 0 alength)
         )
         (= (a main_end pos) (+ (* 2 pos) v))
      )
   )
)