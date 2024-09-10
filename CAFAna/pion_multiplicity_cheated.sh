for a in 0 1
do
    for b in 0 1
    do
        for c in 0 1
        do
            echo $a $b $c
            cafe -bq GArAnaScripts/pion_multiplicity_cheated.C $a $b $c
        done
    done
done