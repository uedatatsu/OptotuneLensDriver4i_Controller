# OptotuneLensDriver4i_Controller
Controller for Optotune focus tunable lens driver 4i

## 01-Firmware _Commands_and_Properties.cpp
### int		Handshake();

### int		FirmwareType();

### int		FirmwareBranch();

### int		PartNumberCommand();

### double	SetUpperSoftwareCurrentLimit(double upperCurrentLimit);
### double	GetUpperSoftwareCurrentLimit();
### double	SetLowerSoftwareCurrentLimit(double lowerCurrentLimit);
### double	GetLowerSoftwareCurrentLimit();
### int		FirmwareVersionRead();
### int		DeviceID_Read();
### int		DeviceID_Write(int deviceID);
### int		SetDriftVariables();
### int		GetDriftVariables();

## 02-Error_Commands.cpp
### int CommunicationError();

## 03-Lens_Commands_and_Properties.cpp
### int		SerialNumber();
### int		SetCurrent(double inputCurrent);
### double	GetCurrent();
### double	GetSignalGeneratorUpperCurrentLimit();
### int		SetSignalGeneratorUpperCurrentLimit(double upperSwingLimit);
### double	GetSignalGeneratorLowerCurrentLimit();
### int		SetSignalGeneratorLowerCurrentLimit(double lowerSwingLimit);
### double	GetSignalGeneratorFrequency();
### int		SetSignalGeneratorFrequency(double frequency);
### int		SetTemperatureLimits(double maximumTemperature, double minimumTemperature);
### std::tuple<double, double>GetTemperatureLimits();
### std::tuple<double, double>GetEEPROMTemperatureValues();
### int		SetEEPROMTemperatureValues(double firstTemperature, double secondTemperature);
### double	GetFocalPower();
### int		SetFocalPower(double focalPower);
### int		SetMaxOutputCurrent(double maxCurrent);
### double	GetMaxOutputCurrent();
### int		SetCalibrationFactor(double currentFactor);
### double	GetCalibrationFactor();
### double	TemperatureReading();
### double	GetStatus();
### double	ReadAddressFromTheEEPROM();
### int		AnalogInputReading();
### int		WriteByteToEEPROM();
### int		ReadEEPROM();

## 04-Operation_Mode.cpp
### int ChangeToSinusoidalSignal();
### int ChangeToRectangularSignal();
### int ChangeToCurrentMode();
### int ChangeToFocalPowerControlledMode();
### int ReadCurrentlyActiveMode();
### int ChangeToAnalogMode();

## 06-Advanced_Commands.cpp
### int GPIOToogle();
### int SetLensDriverToDFUMode();
### int Nodemonstration();
### double InterpoaltionCommand(double current, double temperature);

