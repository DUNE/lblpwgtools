# Step 3 input/output data

The step 3 executables read several inputs from this directory using paths
relative to `lblpwgtools/CAFAna/PRISM/GeoEff` (for example
`data/step3/FileWithCoeffsNuMu.root` and `data/step3/Splines_FDEventRateAtND.root`),
and write their outputs into the current working directory.
`PRISMPred_*.root` are the standard PRISM predictions used by
`macros/step3_etrim/PlotAllOAPosWithCAFLikeCutClean.C`.

Run them from `lblpwgtools/CAFAna/PRISM/GeoEff` (two levels up), for example:

```bash
cd ../..
build/bin/NtupleOutVetoAndTrimE_AssumeEqualEffAtAllOA_NOOscSpectrumNOCAFAna_FDRateAtND <args>
```

With the full CAFAna build, the executables are on your `PATH` after sourcing
`CAFAnaEnv.sh`, so you can drop the `build/bin/` prefix.

The `*.root` files here are ignored by git (`CAFAna/.gitignore`), so they have to
be copied in by hand.
