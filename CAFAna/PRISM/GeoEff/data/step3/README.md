# Step 3 input/output data

The step 3 executables read several inputs by relative filename (for example
`FileWithCoeffsNuMu.root`, `Splines_FDEventRateAtND.root`, and
`PRISMPred_*.root`) and write outputs into the current working directory.

Run them from this directory, for example:

```bash
cd data/step3
../../build-step3/bin/NtupleOutVetoAndTrimE_AssumeEqualEffAtAllOA_NOOscSpectrumNOCAFAna_FDRateAtND <args>
```

If built with `app/Makefile`, the binaries are in `app/`; run
`../../app/<APP>` from this directory instead.
