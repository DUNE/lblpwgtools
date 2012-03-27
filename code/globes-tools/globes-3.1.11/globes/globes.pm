# This file was created automatically by SWIG 1.3.29.
# Don't modify this file, modify the SWIG interface instead.
package globes;
require Exporter;
require DynaLoader;
@ISA = qw(Exporter DynaLoader);
package globesc;
bootstrap globes;
package globes;
@EXPORT = qw( );

# ---------- BASE METHODS -------------

package globes;

sub TIEHASH {
    my ($classname,$obj) = @_;
    return bless $obj, $classname;
}

sub CLEAR { }

sub FIRSTKEY { }

sub NEXTKEY { }

sub FETCH {
    my ($self,$field) = @_;
    my $member_func = "swig_${field}_get";
    $self->$member_func();
}

sub STORE {
    my ($self,$field,$newval) = @_;
    my $member_func = "swig_${field}_set";
    $self->$member_func($newval);
}

sub this {
    my $ptr = shift;
    return tied(%$ptr);
}


# ------- FUNCTION WRAPPERS --------

package globes;

*glbInit = *globesc::glbInit;
*glbClearExperimentList = *globesc::glbClearExperimentList;
*glbDefineAEDLVariable = *globesc::glbDefineAEDLVariable;
*glbDefineAEDLList = *globesc::glbDefineAEDLList;
*glbGetAEDLVariable = *globesc::glbGetAEDLVariable;
*glbClearAEDLVariables = *globesc::glbClearAEDLVariables;
*glbInitExperiment = *globesc::glbInitExperiment;
*glbNameToValue = *globesc::glbNameToValue;
*glbValueToName = *globesc::glbValueToName;
*glbTestReleaseVersion = *globesc::glbTestReleaseVersion;
*glbTestLibraryVersion = *globesc::glbTestLibraryVersion;
*glbVersionOfExperiment = *globesc::glbVersionOfExperiment;
*glbGetFilenameOfExperiment = *globesc::glbGetFilenameOfExperiment;
*glbGetCitationForExperiment = *globesc::glbGetCitationForExperiment;
*glbSetVerbosityLevel = *globesc::glbSetVerbosityLevel;
*glbGetVerbosityLevel = *globesc::glbGetVerbosityLevel;
*glbAllocParams = *globesc::glbAllocParams;
*glbFreeParams = *globesc::glbFreeParams;
*glbDefineParams = *globesc::glbDefineParams;
*glbCopyParams = *globesc::glbCopyParams;
*glbSetOscParams = *globesc::glbSetOscParams;
*glbSetDensityParams = *globesc::glbSetDensityParams;
*glbSetIteration = *globesc::glbSetIteration;
*glbGetOscParams = *globesc::glbGetOscParams;
*glbGetDensityParams = *globesc::glbGetDensityParams;
*glbGetIteration = *globesc::glbGetIteration;
*glbPrintParams = *globesc::glbPrintParams;
*glbClearParamNames = *globesc::glbClearParamNames;
*glbSetParamNames = *globesc::glbSetParamNames;
*glbSetParamName = *globesc::glbSetParamName;
*glbGetParamName = *globesc::glbGetParamName;
*glbFindParamByName = *globesc::glbFindParamByName;
*glbSetOscParamByName = *globesc::glbSetOscParamByName;
*glbGetOscParamByName = *globesc::glbGetOscParamByName;
*glbSetOscillationParameters = *globesc::glbSetOscillationParameters;
*glbSetInputErrors = *globesc::glbSetInputErrors;
*glbGetOscillationParameters = *globesc::glbGetOscillationParameters;
*glbGetInputErrors = *globesc::glbGetInputErrors;
*glbSetCentralValues = *globesc::glbSetCentralValues;
*glbGetCentralValues = *globesc::glbGetCentralValues;
*glbShowRuleRates = *globesc::glbShowRuleRates;
*glbShowChannelRates = *globesc::glbShowChannelRates;
*glbShowChannelProbs = *globesc::glbShowChannelProbs;
*glbPrintDelimiter = *globesc::glbPrintDelimiter;
*glbSetChannelPrintFunction = *globesc::glbSetChannelPrintFunction;
*glbSetPrintDelimiters = *globesc::glbSetPrintDelimiters;
*glbSetRates = *globesc::glbSetRates;
*glbSetNewRates = *globesc::glbSetNewRates;
*glbAllocProjection = *globesc::glbAllocProjection;
*glbFreeProjection = *globesc::glbFreeProjection;
*glbDefineProjection = *globesc::glbDefineProjection;
*glbCopyProjection = *globesc::glbCopyProjection;
*glbSetProjectionFlag = *globesc::glbSetProjectionFlag;
*glbSetDensityProjectionFlag = *globesc::glbSetDensityProjectionFlag;
*glbGetProjectionFlag = *globesc::glbGetProjectionFlag;
*glbGetDensityProjectionFlag = *globesc::glbGetDensityProjectionFlag;
*glbPrintProjection = *globesc::glbPrintProjection;
*glbSetProjectionFlagByName = *globesc::glbSetProjectionFlagByName;
*glbGetProjectionFlagByName = *globesc::glbGetProjectionFlagByName;
*glbSetProjection = *globesc::glbSetProjection;
*glbGetProjection = *globesc::glbGetProjection;
*glbChiSys = *globesc::glbChiSys;
*glbChiTheta13 = *globesc::glbChiTheta13;
*glbChiTheta12 = *globesc::glbChiTheta12;
*glbChiTheta23 = *globesc::glbChiTheta23;
*glbChiDelta = *globesc::glbChiDelta;
*glbChiDm21 = *globesc::glbChiDm21;
*glbChiDm31 = *globesc::glbChiDm31;
*glbChiTheta13Delta = *globesc::glbChiTheta13Delta;
*glbChiNP = *globesc::glbChiNP;
*glbChiAll = *globesc::glbChiAll;
*glbLoadProfileData = *globesc::glbLoadProfileData;
*glbStaceyProfile = *globesc::glbStaceyProfile;
*glbAverageDensityProfile = *globesc::glbAverageDensityProfile;
*glbGetProfileDataInExperiment = *globesc::glbGetProfileDataInExperiment;
*glbSetProfileDataInExperiment = *globesc::glbSetProfileDataInExperiment;
*glbSetBaselineInExperiment = *globesc::glbSetBaselineInExperiment;
*glbGetProfileTypeInExperiment = *globesc::glbGetProfileTypeInExperiment;
*glbGetBaselineInExperiment = *globesc::glbGetBaselineInExperiment;
*glbSetTargetMass = *globesc::glbSetTargetMass;
*glbSetSourcePower = *globesc::glbSetSourcePower;
*glbSetRunningTime = *globesc::glbSetRunningTime;
*glbSetFilterStateInExperiment = *globesc::glbSetFilterStateInExperiment;
*glbSetFilterInExperiment = *globesc::glbSetFilterInExperiment;
*glbCompensateFilterInExperiment = *globesc::glbCompensateFilterInExperiment;
*glbOptimizeSmearingMatrixInExperiment = *globesc::glbOptimizeSmearingMatrixInExperiment;
*glbGetTargetMass = *globesc::glbGetTargetMass;
*glbGetSourcePower = *globesc::glbGetSourcePower;
*glbGetRunningTime = *globesc::glbGetRunningTime;
*glbGetFilterStateInExperiment = *globesc::glbGetFilterStateInExperiment;
*glbGetFilterInExperiment = *globesc::glbGetFilterInExperiment;
*glbGetEminEmax = *globesc::glbGetEminEmax;
*glbGetEnergyWindow = *globesc::glbGetEnergyWindow;
*glbGetEnergyWindowBins = *globesc::glbGetEnergyWindowBins;
*glbSetEnergyWindow = *globesc::glbSetEnergyWindow;
*glbGetNumberOfSamplingPoints = *globesc::glbGetNumberOfSamplingPoints;
*glbGetNumberOfBins = *globesc::glbGetNumberOfBins;
*glbGetNumberOfRules = *globesc::glbGetNumberOfRules;
*glbGetNumberOfChannels = *globesc::glbGetNumberOfChannels;
*glbGetBinSizeListPtr = *globesc::glbGetBinSizeListPtr;
*glbGetBinCentersListPtr = *globesc::glbGetBinCentersListPtr;
*glbGetSamplingStepsizeListPtr = *globesc::glbGetSamplingStepsizeListPtr;
*glbGetSamplingPointsListPtr = *globesc::glbGetSamplingPointsListPtr;
*glbGetLengthOfRule = *globesc::glbGetLengthOfRule;
*glbGetChannelInRule = *globesc::glbGetChannelInRule;
*glbGetCoefficientInRule = *globesc::glbGetCoefficientInRule;
*glbSetCoefficientInRule = *globesc::glbSetCoefficientInRule;
*glbGetChannelFlux = *globesc::glbGetChannelFlux;
*glbGetNumberOfFluxes = *globesc::glbGetNumberOfFluxes;
*glbFlux = *globesc::glbFlux;
*glbXSection = *globesc::glbXSection;
*glbDefineChiFunction = *globesc::glbDefineChiFunction;
*glbSetChiFunction = *globesc::glbSetChiFunction;
*glbSwitchSystematics = *globesc::glbSwitchSystematics;
*glbSetSignalErrors = *globesc::glbSetSignalErrors;
*glbSetSignalStartingValues = *globesc::glbSetSignalStartingValues;
*glbSetBGErrors = *globesc::glbSetBGErrors;
*glbSetBGStartingValues = *globesc::glbSetBGStartingValues;
*glbSetSysErrorsList = *globesc::glbSetSysErrorsList;
*glbSetSysStartingValuesList = *globesc::glbSetSysStartingValuesList;
*glbGetSysDim = *globesc::glbGetSysDim;
*glbGetSysDimInExperiment = *globesc::glbGetSysDimInExperiment;
*glbGetChiFunction = *globesc::glbGetChiFunction;
*glbGetChiFunctionPtr = *globesc::glbGetChiFunctionPtr;
*glbGetChiFunctionPtrInExperiment = *globesc::glbGetChiFunctionPtrInExperiment;
*glbGetSysOnOffState = *globesc::glbGetSysOnOffState;
*glbGetSignalErrors = *globesc::glbGetSignalErrors;
*glbGetSignalStartingValues = *globesc::glbGetSignalStartingValues;
*glbGetBGErrors = *globesc::glbGetBGErrors;
*glbGetBGStartingValues = *globesc::glbGetBGStartingValues;
*glbGetSysErrorsListPtr = *globesc::glbGetSysErrorsListPtr;
*glbGetSysStartingValuesListPtr = *globesc::glbGetSysStartingValuesListPtr;
*glbShiftEnergyScale = *globesc::glbShiftEnergyScale;
*glbRegisterProbabilityEngine = *globesc::glbRegisterProbabilityEngine;
*glbGetNumOfOscParams = *globesc::glbGetNumOfOscParams;
*glbTotalRuleRate = *globesc::glbTotalRuleRate;
*glbGetChannelRatePtr = *globesc::glbGetChannelRatePtr;
*glbGetRuleRatePtr = *globesc::glbGetRuleRatePtr;
*glbGetSignalRatePtr = *globesc::glbGetSignalRatePtr;
*glbGetBGRatePtr = *globesc::glbGetBGRatePtr;
*glbGetChannelFitRatePtr = *globesc::glbGetChannelFitRatePtr;
*glbGetSignalFitRatePtr = *globesc::glbGetSignalFitRatePtr;
*glbGetBGFitRatePtr = *globesc::glbGetBGFitRatePtr;
*glbGetEfficiencyPtr = *globesc::glbGetEfficiencyPtr;
*glbGetBackgroundPtr = *globesc::glbGetBackgroundPtr;
*glbVacuumProbability = *globesc::glbVacuumProbability;
*glbConstantDensityProbability = *globesc::glbConstantDensityProbability;
*glbProfileProbability = *globesc::glbProfileProbability;
*glbFilteredConstantDensityProbability = *globesc::glbFilteredConstantDensityProbability;
*glbSelectMinimizer = *globesc::glbSelectMinimizer;
*glbChiTheta = *globesc::glbChiTheta;
*glbChiDms = *globesc::glbChiDms;
*glbChiDm = *globesc::glbChiDm;
*glbChiThetaDelta = *globesc::glbChiThetaDelta;
*glbGetUserData = *globesc::glbGetUserData;
*glbGetChannelRates = *globesc::glbGetChannelRates;
*glbGetProfileData = *globesc::glbGetProfileData;
*glbResetRateStack = *globesc::glbResetRateStack;
*glbSetFilterState = *globesc::glbSetFilterState;
*glbGetFilterState = *globesc::glbGetFilterState;
*glbSetFilter = *globesc::glbSetFilter;
*glbGetFilter = *globesc::glbGetFilter;
*glbSetErrorDim = *globesc::glbSetErrorDim;
*glbGetErrorDim = *globesc::glbGetErrorDim;
*glbGetNormalizationInRule = *globesc::glbGetNormalizationInRule;
*glbSetBGCenters = *globesc::glbSetBGCenters;
*glbGetBGCenters = *globesc::glbGetBGCenters;
*glbSetStartingValues = *globesc::glbSetStartingValues;
*glbGetStartingValues = *globesc::glbGetStartingValues;
*glbGetNumberOfSimBins = *globesc::glbGetNumberOfSimBins;
*PInit = *globesc::PInit;

# ------- VARIABLE STUBS --------

package globes;

*__GLOBES_H = *globesc::__GLOBES_H;
*GLB_NU_FLAVOURS = *globesc::GLB_NU_FLAVOURS;
*GLB_THETA_12 = *globesc::GLB_THETA_12;
*GLB_THETA_13 = *globesc::GLB_THETA_13;
*GLB_THETA_23 = *globesc::GLB_THETA_23;
*GLB_DELTA_CP = *globesc::GLB_DELTA_CP;
*GLB_DM_21 = *globesc::GLB_DM_21;
*GLB_DM_31 = *globesc::GLB_DM_31;
*GLB_OFF = *globesc::GLB_OFF;
*GLB_ON = *globesc::GLB_ON;
*GLB_W_EFF = *globesc::GLB_W_EFF;
*GLB_WO_EFF = *globesc::GLB_WO_EFF;
*GLB_W_BG = *globesc::GLB_W_BG;
*GLB_WO_BG = *globesc::GLB_WO_BG;
*GLB_W_COEFF = *globesc::GLB_W_COEFF;
*GLB_WO_COEFF = *globesc::GLB_WO_COEFF;
*GLB_FIXED = *globesc::GLB_FIXED;
*GLB_FREE = *globesc::GLB_FREE;
*GLB_PRE = *globesc::GLB_PRE;
*GLB_POST = *globesc::GLB_POST;
*GLB_MIN_NESTED_POWELL = *globesc::GLB_MIN_NESTED_POWELL;
*GLB_MIN_POWELL = *globesc::GLB_MIN_POWELL;
*GLB_EFF = *globesc::GLB_EFF;
*GLB_BG = *globesc::GLB_BG;
*GLB_SIG = *globesc::GLB_SIG;
*GLB_ALL = *globesc::GLB_ALL;
*GLB_EARTH_RADIUS = *globesc::GLB_EARTH_RADIUS;
*GLB_EV_TO_KM_FACTOR = *globesc::GLB_EV_TO_KM_FACTOR;
*GLB_MAX_EXP = *globesc::GLB_MAX_EXP;
*glb_path_vector = *globesc::glb_path_vector;
*glb_path_vector_length = *globesc::glb_path_vector_length;
*glb_num_of_exps = *globesc::glb_num_of_exps;
*glb_single_experiment_number = *globesc::glb_single_experiment_number;
*glb_experiment_list = *globesc::glb_experiment_list;
*glb_rule_number = *globesc::glb_rule_number;
1;
