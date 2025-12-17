#pragma once


template<typename T>
inline bool IsInvalidPtr(T ptr) {
#if defined(aarch64)
	return (((uintptr_t)ptr & 0xFFFFFFFFFFFFFF) <= 0x1000000000);
#elif defined(arm)
	return ((uintptr_t)ptr <= 0x10000000);
#else
	return (uintptr_t)ptr <= 0x10000000;
#endif
};

bool is_visible(uintptr_t mesh)
{
	float last_sumbit_time = read<float>(mesh + Offsets::LastSubmitTime);
	float last_render_time = read<float>(mesh + Offsets::LastRenderTime);
	return last_render_time + 0.06f >= last_sumbit_time;
}

inline Vector3 GetEntityBoneFast(uintptr_t Mesh, uintptr_t BoneArray, int BoneId)
{
	FTransform Bone = read<FTransform>(BoneArray + (BoneId * 0x30));
	uintptr_t FunctionAddress = read<uintptr_t>(0x15432F5A8);
	FTransform ComponentToWorld = read<FTransform>(Mesh + Offsets::ComponentToWorld);
	uint16_t encryptionFlag = read<uint16_t>(Mesh + Offsets::ComponentToWorld + 0x30);

	//std::cerr << "Encryption Flag: " << std::hex << encryptionFlag << std::dec << std::endl;

	if (encryptionFlag != 0xFFFF)
	{
		EmulateFunction(FunctionAddress, (uintptr_t*)&ComponentToWorld.Translation, 0x10, encryptionFlag);
	}

	D3DMATRIX Matrix = MatrixMultiplication(Bone.ToMatrixWithScale(), ComponentToWorld.ToMatrixWithScale());

	return Vector3(Matrix._41, Matrix._42, Matrix._43);
}

inline Vector3 GetEntityBone(uintptr_t Mesh, uintptr_t BoneArray, int BoneId)
{
	if (wegame)
		return GetEntityBoneFast(Mesh, BoneArray, BoneId);

	FTransform Bone = read<FTransform>(BoneArray + (BoneId * 0x30));

	FTransform ComponentToWorld = read<FTransform>(Mesh + Offsets::ComponentToWorld);

	D3DMATRIX Matrix = MatrixMultiplication(Bone.ToMatrixWithScale(), ComponentToWorld.ToMatrixWithScale());

	return Vector3(Matrix._41, Matrix._42, Matrix._43);
}

inline Vector3 GetEntityLocation(uintptr_t Mesh, uintptr_t BoneArray, int BoneId)
{
	FTransform Bone = read<FTransform>(BoneArray + (BoneId * 0x30));
	uintptr_t FunctionAddress = read<uintptr_t>(0x15432F5A8);
	FTransform ComponentToWorld = read<FTransform>(Mesh + Offsets::ComponentToWorld);
	uint16_t encryptionFlag = read<uint16_t>(Mesh + Offsets::ComponentToWorld + 0x30);

	//std::cerr << "Encryption Flag: " << std::hex << encryptionFlag << std::dec << std::endl;

	if (encryptionFlag != 0xFFFF)
	{
		EmulateFunction(FunctionAddress, (uintptr_t*)&ComponentToWorld.Translation, 0x10, encryptionFlag);
	}

	D3DMATRIX Matrix = MatrixMultiplication(Bone.ToMatrixWithScale(), ComponentToWorld.ToMatrixWithScale());

	return Vector3(Matrix._41, Matrix._42, Matrix._43);
}


inline Vector2 ProjectWorldToScreen(FMinimalViewInfo ViewInfo, Vector3 World)
{
	Vector3 ScreenLocation(0, 0, 0);
	Vector3 Rot = Vector3(ViewInfo.Rotation.Pitch, ViewInfo.Rotation.Yaw, ViewInfo.Rotation.Roll);
	Vector3 CamPos = Vector3(ViewInfo.Location.X, ViewInfo.Location.Y, ViewInfo.Location.Z);
	const D3DMATRIX TempMatrix = Matrix(Rot, Vector3(0, 0, 0));

	Vector3 VAxisX(TempMatrix.m[0][0], TempMatrix.m[0][1], TempMatrix.m[0][2]);
	Vector3 VAxisY(TempMatrix.m[1][0], TempMatrix.m[1][1], TempMatrix.m[1][2]);
	Vector3 VAxisZ(TempMatrix.m[2][0], TempMatrix.m[2][1], TempMatrix.m[2][2]);

	Vector3 VDelta = World - CamPos;
	Vector3 VTransformed = Vector3(VDelta.dot(VAxisY), VDelta.dot(VAxisZ), VDelta.dot(VAxisX));

	if (VTransformed.z < 1.f)
		VTransformed.z = 1.f;

	const float FOVDegToRad = static_cast<float>(3.14159265358979323846) / 360.f;
	int CenterX = GetSystemMetrics(SM_CXSCREEN) / 2;
	int CenterY = GetSystemMetrics(SM_CYSCREEN) / 2;
	ScreenLocation.x = CenterX + VTransformed.x * (CenterX / tanf(
		ViewInfo.FOV * FOVDegToRad)) / VTransformed.z;
	ScreenLocation.y = CenterY - VTransformed.y * (CenterX / tanf(
		ViewInfo.FOV * FOVDegToRad)) / VTransformed.z;

	return Vector2(ScreenLocation.x, ScreenLocation.y);
}


bool is_in_fov(const Vector2& target2D, float screenWidth, float screenHeight)
{
	float distance = get_cross_distance(target2D.x, target2D.y, screenWidth / 2, screenHeight / 2);

	return distance <= settings::fov && target2D.x != 0 && target2D.y != 0;
}

ImVec2 calc_aim(const Vector2& target2D, const ImVec2& screenCenter, int aimSpeed, float screenWidth, float screenHeight)
{
	float targetX = 0.0f;
	float targetY = 0.0f;

	if (target2D.x != 0)
	{
		if (target2D.x > screenCenter.x)
		{
			targetX = -(screenCenter.x - target2D.x);
			targetX /= aimSpeed;
			if (targetX + screenCenter.x > screenCenter.x * 2) targetX = 0;
		}

		if (target2D.x < screenCenter.x)
		{
			targetX = target2D.x - screenCenter.x;
			targetX /= aimSpeed;
			if (targetX + screenCenter.x < 0) targetX = 0;
		}
	}

	if (target2D.y != 0)
	{
		if (target2D.y > screenCenter.y)
		{
			targetY = -(screenCenter.y - target2D.y);
			targetY /= aimSpeed;
			if (targetY + screenCenter.y > screenCenter.y * 2) targetY = 0;
		}

		if (target2D.y < screenCenter.y)
		{
			targetY = target2D.y - screenCenter.y;
			targetY /= aimSpeed;
			if (targetY + screenCenter.y < 0) targetY = 0;
		}
	}

	return ImVec2(targetX, targetY);
}

Vector3 CalculateAngle(Vector3 source, Vector3 target)
{
	Vector3 delta = target - source;

	float hypotenuse = sqrt(delta.x * delta.x + delta.y * delta.y); // Distance in 2D plane

	Vector3 angle;
	angle.x = -atan2(delta.z, hypotenuse) * (180.0f / M_PI); // Pitch
	angle.y = atan2(delta.y, delta.x) * (180.0f / M_PI);     // Yaw
	angle.z = 0.0f; // Roll is always zero in most games

	return angle;
}

inline void ClampAngles(Vector3& angles) {
	if (angles.x > 89.0f) angles.x = 89.0f;
	if (angles.x < -89.0f) angles.x = -89.0f;

	while (angles.y > 180.0f) angles.y -= 360.0f;
	while (angles.y < -180.0f) angles.y += 360.0f;

	angles.z = 0.0f;
}

inline void NormalizeAngles(Vector3& angles) {
	angles.x = fmodf(angles.x + 180.f, 360.f) - 180.f;
	angles.y = fmodf(angles.y + 180.f, 360.f) - 180.f;
	angles.z = 0.f;
}

inline Vector3 StableSmoothAngles(Vector3 current, Vector3 target, float smoothFactor) {
	Vector3 delta = target - current;
	ClampAngles(delta);

	float verticalSmooth = smoothFactor * 1.2f;
	float horizontalSmooth = smoothFactor * 0.8f;

	Vector3 smoothed;
	smoothed.x = current.x + delta.x / verticalSmooth;
	smoothed.y = current.y + delta.y / horizontalSmooth;
	smoothed.z = 0.0f;

	ClampAngles(smoothed);
	return smoothed;
}

Vector3 PredictTargetPosition(Vector3 TargetPosition, Vector3 TargetVelocity, Vector3 ShooterPosition, float BulletSpeed) {
	float DistanceToTarget = Pointer::CameraCache.POV.Location.distance(TargetPosition) * 0.01f;
	float TimeToHit = DistanceToTarget / BulletSpeed;
	return TargetPosition + (TargetVelocity * TimeToHit);
}

Vector3 GetVelocity(uintptr_t pawn)
{
	uintptr_t CharacterMovement = read<uintptr_t>(pawn + Offsets::CharacterMovement);
	if (!CharacterMovement) return {};

	Vector3 LastUpdateVelocity = read<Vector3>(CharacterMovement + Offsets::LastUpdateVelocity);
	return LastUpdateVelocity;
}

void aimbot(uintptr_t target_mesh, uintptr_t target_entity, uintptr_t target_root_comp, uintptr_t BoneArray)
{
	if (!target_mesh || !target_entity)
		return;

	if (settings::vischeck && !is_visible(target_mesh))
		return;

	// Choose the target bone and the corresponding local player's bone based on the settings hitbox
	int targetBone;
	switch (settings::hitbox) {
	case 0: targetBone = Head; break;
	case 1: targetBone = Neck; break;
	case 2: targetBone = Pelvis; break;
	default: targetBone = Head; break;
	}

	Vector3 targetBonePos = GetEntityBone(target_mesh, BoneArray, targetBone);

	// Get bullet speed
	uintptr_t CharacterMovement = read<uintptr_t>(Pointer::LocalPawn + Offsets::CharacterMovement);
	float BulletSpeed = read<float>(CharacterMovement + Offsets::MaxFlySpeed);

	// Calculate prediction
	Vector3 predictedPos = settings::prediction ?
		PredictTargetPosition(targetBonePos, GetVelocity(target_entity), Vector3(Pointer::CameraCache.POV.Location.X, Pointer::CameraCache.POV.Location.Y, Pointer::CameraCache.POV.Location.Z), BulletSpeed) :
		targetBonePos;

	// Check if target is in FOV
	Vector2 screenPos = ProjectWorldToScreen(Pointer::CameraCache.POV, predictedPos);
	if (!is_in_fov(screenPos, width, height)) {
		Pointer::closest_distance = FLT_MAX;
		Pointer::closest_mesh = NULL;
		Pointer::closest_entity = NULL;
		Pointer::closest_entityroot_comp = NULL;
		return;
	}

	// Calculate target angles
	Vector3 targetAngles = AimAtVector(predictedPos, Vector3(Pointer::CameraCache.POV.Location.X, Pointer::CameraCache.POV.Location.Y, Pointer::CameraCache.POV.Location.Z));
	NormalizeAngles(targetAngles);

	// Get current control rotation
	Vector3 currentRotation = read<Vector3>(Pointer::PlayerController + Offsets::ControlRotation);
	NormalizeAngles(currentRotation);

	// Apply smoothing
	Vector3 smoothedAngles = StableSmoothAngles(currentRotation, targetAngles, settings::smoothness);

	// Write new rotation
	write<Vector2>(Pointer::PlayerController + Offsets::ControlRotation, Vector2(smoothedAngles.x, smoothedAngles.y));
}

std::string wchar2char(const wchar_t* wstr) {
	if (!wstr) return "";

	int len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, nullptr, 0, nullptr, nullptr);
	if (len == 0) return "";

	std::string str(len - 1, '\0');
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, &str[0], len, nullptr, nullptr);

	return str;
}

float gethealth(uintptr_t Enemy, bool maxreturn)
{
	uintptr_t HealthComp = read<uintptr_t>(Enemy + Offsets::HealthComp);
	if (!HealthComp)
	{
		//printf("HealthComp: %u\n", HealthComp);
		return 0;
	}

	uintptr_t HealthSet = read<uintptr_t>(HealthComp + Offsets::HealthSet);
	if (!HealthSet)
	{
		//printf("HealthSet: %u\n", HealthSet);
		return 0;
	}

	float MaxHealth = read<float>(HealthSet + Offsets::MaxHealth + Offsets::CurrentValue);
	if (maxreturn)
		return MaxHealth;


	float Health = read<float>(HealthSet + Offsets::Health + Offsets::CurrentValue);
	if (!Health)
	{
		return 0;
	}

	return Health;
}

float getarmor(uintptr_t Enemy, bool maxreturn)
{
	uintptr_t HealthComp = read<uintptr_t>(Enemy + Offsets::HealthComp);
	if (!HealthComp)
	{
		//printf("HealthComp: %u\n", HealthComp);
		return 0;
	}

	uintptr_t HealthSet = read<uintptr_t>(HealthComp + Offsets::HealthSet);
	if (!HealthSet)
	{
		//printf("HealthSet: %u\n", HealthSet);
		return 0;
	}

	float ArmorHealth = read<float>(HealthSet + Offsets::ArmorHealth + Offsets::CurrentValue);
	float MaxArmorHealth = read<float>(HealthSet + Offsets::MaxArmorHealth + Offsets::CurrentValue);

	if (maxreturn)
		return MaxArmorHealth;

	return ArmorHealth;
}

/*

struct PESection {
	IMAGE_SECTION_HEADER header;
	std::vector<BYTE> content;
	DWORD initialSize;
};

class PEFile {
public:
	std::vector<PESection> sections;
	IMAGE_DOS_HEADER dosHeader;
	union {
		IMAGE_NT_HEADERS32 ntHeaders32;
		IMAGE_NT_HEADERS64 ntHeaders64;
	};
	std::vector<BYTE> dosStub;
	bool is64Bit;

	void AlignSectionHeaders() {
		DWORD fileAlignment = is64Bit ? ntHeaders64.OptionalHeader.FileAlignment
			: ntHeaders32.OptionalHeader.FileAlignment;

		for (auto& section : sections) {
			section.header.SizeOfRawData = AlignTo(section.content.size(), fileAlignment);
		}
	}

	void FixPEHeader() {
		if (is64Bit) {
			ntHeaders64.OptionalHeader.SizeOfImage = CalculateSizeOfImage();
			ntHeaders64.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].Size = 0;
			ntHeaders64.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].VirtualAddress = 0;
		}
		else {
			ntHeaders32.OptionalHeader.SizeOfImage = CalculateSizeOfImage();
			ntHeaders32.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].Size = 0;
			ntHeaders32.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].VirtualAddress = 0;
		}
	}

private:
	DWORD AlignTo(DWORD size, DWORD alignment) {
		return (size + alignment - 1) & ~(alignment - 1);
	}

	DWORD CalculateSizeOfImage() {
		DWORD sizeOfImage = is64Bit ? ntHeaders64.OptionalHeader.SectionAlignment
			: ntHeaders32.OptionalHeader.SectionAlignment;

		for (const auto& section : sections) {
			DWORD alignedSize = AlignTo(section.header.Misc.VirtualSize,  // Changed from VirtualSize to Misc.VirtualSize
				is64Bit ? ntHeaders64.OptionalHeader.SectionAlignment
				: ntHeaders32.OptionalHeader.SectionAlignment);
			sizeOfImage += alignedSize;
		}
		return sizeOfImage;
	}
};
void FixImportThunks(PEFile& peFile, const IMAGE_IMPORT_DESCRIPTOR& importDesc, const std::string& dllName) {
	// We need to handle both INT (OriginalFirstThunk) and IAT (FirstThunk)
	DWORD intRVA = importDesc.OriginalFirstThunk;
	DWORD iatRVA = importDesc.FirstThunk;

	// If OriginalFirstThunk is 0, use FirstThunk for both
	if (intRVA == 0) {
		intRVA = iatRVA;
	}

	// Find sections containing the INT and IAT
	int intSectionIndex = -1;
	DWORD intOffset = 0;
	int iatSectionIndex = -1;
	DWORD iatOffset = 0;

	// Find INT section
	for (int i = 0; i < peFile.sections.size(); i++) {
		DWORD sectionStart = peFile.sections[i].header.VirtualAddress;
		DWORD sectionEnd = sectionStart + peFile.sections[i].header.Misc.VirtualSize;

		if (intRVA >= sectionStart && intRVA < sectionEnd) {
			intSectionIndex = i;
			intOffset = intRVA - sectionStart;
			break;
		}
	}

	// Find IAT section
	for (int i = 0; i < peFile.sections.size(); i++) {
		DWORD sectionStart = peFile.sections[i].header.VirtualAddress;
		DWORD sectionEnd = sectionStart + peFile.sections[i].header.Misc.VirtualSize;

		if (iatRVA >= sectionStart && iatRVA < sectionEnd) {
			iatSectionIndex = i;
			iatOffset = iatRVA - sectionStart;
			break;
		}
	}

	// If we couldn't find the sections, we can't fix the imports
	if (intSectionIndex == -1 || iatSectionIndex == -1) {
		std::cerr << "Could not find INT or IAT section for " << dllName << std::endl;
		return;
	}

	PESection& intSection = peFile.sections[intSectionIndex];
	PESection& iatSection = peFile.sections[iatSectionIndex];

	// Process thunks
	DWORD thunkSize = peFile.is64Bit ? sizeof(IMAGE_THUNK_DATA64) : sizeof(IMAGE_THUNK_DATA32);
	DWORD currentIntOffset = intOffset;
	DWORD currentIatOffset = iatOffset;
	int importCount = 0;

	while (true) {
		// Make sure we don't read past the end of sections
		if (currentIntOffset + thunkSize > intSection.content.size() ||
			currentIatOffset + thunkSize > iatSection.content.size()) {
			break;
		}

		// Read the thunk value from INT
		ULONGLONG ordinalOrRVA = 0;
		if (peFile.is64Bit) {
			IMAGE_THUNK_DATA64 thunkData;
			memcpy(&thunkData, intSection.content.data() + currentIntOffset, sizeof(IMAGE_THUNK_DATA64));
			ordinalOrRVA = thunkData.u1.AddressOfData;

			// Check for end of thunks
			if (ordinalOrRVA == 0) {
				break;
			}
		}
		else {
			IMAGE_THUNK_DATA32 thunkData;
			memcpy(&thunkData, intSection.content.data() + currentIntOffset, sizeof(IMAGE_THUNK_DATA32));
			ordinalOrRVA = thunkData.u1.AddressOfData;

			// Check for end of thunks
			if (ordinalOrRVA == 0) {
				break;
			}
		}

		// Check if this is an ordinal import (high bit set)
		bool isOrdinal = (ordinalOrRVA & (peFile.is64Bit ? 0x8000000000000000 : 0x80000000)) != 0;

		if (isOrdinal) {
			// Handle ordinal import - nothing to fix here
			WORD ordinal = static_cast<WORD>(ordinalOrRVA & 0xFFFF);
			std::cout << "  Ordinal import: " << ordinal << std::endl;
		}
		else {
			// Handle named import - need to resolve the import by name
			DWORD importNameRVA = static_cast<DWORD>(ordinalOrRVA);

			// Find the section containing the import name
			int nameSectionIndex = -1;
			DWORD nameOffset = 0;

			for (int i = 0; i < peFile.sections.size(); i++) {
				DWORD sectionStart = peFile.sections[i].header.VirtualAddress;
				DWORD sectionEnd = sectionStart + peFile.sections[i].header.Misc.VirtualSize;

				if (importNameRVA >= sectionStart && importNameRVA < sectionEnd) {
					nameSectionIndex = i;
					nameOffset = importNameRVA - sectionStart;
					break;
				}
			}

			if (nameSectionIndex != -1) {
				PESection& nameSection = peFile.sections[nameSectionIndex];

				// Check if we have enough space for the IMAGE_IMPORT_BY_NAME structure
				if (nameOffset + sizeof(WORD) < nameSection.content.size()) {
					// Read the hint
					WORD hint;
					memcpy(&hint, nameSection.content.data() + nameOffset, sizeof(WORD));

					// Read the name (null-terminated string)
					std::string importName;
					DWORD nameStringOffset = nameOffset + sizeof(WORD);

					while (nameStringOffset < nameSection.content.size() &&
						nameSection.content[nameStringOffset] != 0) {
						importName += static_cast<char>(nameSection.content[nameStringOffset]);
						nameStringOffset++;
					}

					if (!importName.empty()) {
						std::cout << "  Named import: " << importName << " (hint: " << hint << ")" << std::endl;
					}
				}
			}
		}

		// For IAT entries, they might have been resolved to actual addresses
		// We need to update them to point to the thunk data again
		if (intSectionIndex != iatSectionIndex || intOffset != iatOffset) {
			if (peFile.is64Bit) {
				IMAGE_THUNK_DATA64 thunkData;
				memcpy(&thunkData, intSection.content.data() + currentIntOffset, sizeof(IMAGE_THUNK_DATA64));
				memcpy(iatSection.content.data() + currentIatOffset, &thunkData, sizeof(IMAGE_THUNK_DATA64));
			}
			else {
				IMAGE_THUNK_DATA32 thunkData;
				memcpy(&thunkData, intSection.content.data() + currentIntOffset, sizeof(IMAGE_THUNK_DATA32));
				memcpy(iatSection.content.data() + currentIatOffset, &thunkData, sizeof(IMAGE_THUNK_DATA32));
			}
		}

		// Move to the next thunk
		currentIntOffset += thunkSize;
		currentIatOffset += thunkSize;
		importCount++;
	}

	std::cout << "  Fixed " << importCount << " imports for " << dllName << std::endl;
}


void FixImportDirectory(PEFile& peFile) {
	// Get the import directory information
	IMAGE_DATA_DIRECTORY importDir;

	if (peFile.is64Bit) {
		importDir = peFile.ntHeaders64.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
	}
	else {
		importDir = peFile.ntHeaders32.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
	}

	// If no import directory, nothing to fix
	if (importDir.VirtualAddress == 0 || importDir.Size == 0) {
		std::cout << "No import directory found." << std::endl;
		return;
	}

	// Find which section contains the import directory
	int importSectionIndex = -1;
	for (int i = 0; i < peFile.sections.size(); i++) {
		DWORD sectionStart = peFile.sections[i].header.VirtualAddress;
		DWORD sectionEnd = sectionStart + peFile.sections[i].header.Misc.VirtualSize;

		if (importDir.VirtualAddress >= sectionStart && importDir.VirtualAddress < sectionEnd) {
			importSectionIndex = i;
			break;
		}
	}

	if (importSectionIndex == -1) {
		std::cerr << "Import directory not found in any section." << std::endl;
		return;
	}

	std::cout << "Import directory found in section: " << importSectionIndex << std::endl;

	// Get section that contains the import directory
	PESection& importSection = peFile.sections[importSectionIndex];
	DWORD sectionOffset = importDir.VirtualAddress - importSection.header.VirtualAddress;

	// Check if we have enough data in the section
	if (sectionOffset >= importSection.content.size()) {
		std::cerr << "Import directory offset exceeds section size." << std::endl;
		return;
	}

	// Iterate through import descriptors
	DWORD currentDescriptorOffset = sectionOffset;
	std::vector<std::string> processedDlls; // Track processed DLLs to avoid duplicates

	while (true) {
		// Make sure we don't read past the end of the section
		if (currentDescriptorOffset + sizeof(IMAGE_IMPORT_DESCRIPTOR) > importSection.content.size()) {
			std::cerr << "Warning: Import descriptor would exceed section bounds." << std::endl;
			break;
		}

		// Get the import descriptor
		IMAGE_IMPORT_DESCRIPTOR importDesc;
		memcpy(&importDesc, importSection.content.data() + currentDescriptorOffset, sizeof(IMAGE_IMPORT_DESCRIPTOR));

		// Check if we've reached the end of import descriptors (null entry)
		if (importDesc.Name == 0 && importDesc.FirstThunk == 0) {
			break;
		}

		// Get the DLL name
		DWORD dllNameRVA = importDesc.Name;

		// Find the section containing the DLL name
		int dllNameSectionIndex = -1;
		DWORD dllNameOffset = 0;

		for (int i = 0; i < peFile.sections.size(); i++) {
			DWORD sectionStart = peFile.sections[i].header.VirtualAddress;
			DWORD sectionEnd = sectionStart + peFile.sections[i].header.Misc.VirtualSize;

			if (dllNameRVA >= sectionStart && dllNameRVA < sectionEnd) {
				dllNameSectionIndex = i;
				dllNameOffset = dllNameRVA - sectionStart;
				break;
			}
		}

		// If we found the section with the DLL name
		if (dllNameSectionIndex != -1) {
			PESection& dllNameSection = peFile.sections[dllNameSectionIndex];

			// Make sure the offset is valid
			if (dllNameOffset < dllNameSection.content.size()) {
				// Read the DLL name (null-terminated string)
				std::string dllName;
				DWORD namePos = dllNameOffset;

				while (namePos < dllNameSection.content.size() && dllNameSection.content[namePos] != 0) {
					dllName += static_cast<char>(dllNameSection.content[namePos]);
					namePos++;
				}

				if (!dllName.empty()) {
					std::cout << "Processing imports from: " << dllName << std::endl;
					processedDlls.push_back(dllName);

					// Now fix the thunks for this DLL
					FixImportThunks(peFile, importDesc, dllName);
				}
			}
		}

		// Move to the next import descriptor
		currentDescriptorOffset += sizeof(IMAGE_IMPORT_DESCRIPTOR);
	}

	std::cout << "Processed " << processedDlls.size() << " DLLs in import table." << std::endl;
}
void FixIATForDescriptor(PEFile& peFile, const IMAGE_IMPORT_DESCRIPTOR& importDesc,
	uintptr_t actualBaseAddress, uintptr_t expectedBaseAddress) {
	DWORD iatRVA = importDesc.FirstThunk;

	// Find the section containing the IAT
	int iatSectionIndex = -1;
	DWORD iatOffset = 0;

	for (int i = 0; i < peFile.sections.size(); i++) {
		DWORD sectionStart = peFile.sections[i].header.VirtualAddress;
		DWORD sectionEnd = sectionStart + peFile.sections[i].header.Misc.VirtualSize;

		if (iatRVA >= sectionStart && iatRVA < sectionEnd) {
			iatSectionIndex = i;
			iatOffset = iatRVA - sectionStart;
			break;
		}
	}

	if (iatSectionIndex == -1) {
		return;
	}

	PESection& iatSection = peFile.sections[iatSectionIndex];
	DWORD currentOffset = iatOffset;
	DWORD thunkSize = peFile.is64Bit ? sizeof(IMAGE_THUNK_DATA64) : sizeof(IMAGE_THUNK_DATA32);

	// Process each IAT entry
	while (currentOffset + thunkSize <= iatSection.content.size()) {
		if (peFile.is64Bit) {
			IMAGE_THUNK_DATA64 thunkData;
			memcpy(&thunkData, iatSection.content.data() + currentOffset, sizeof(IMAGE_THUNK_DATA64));

			// Check for end of thunks
			if (thunkData.u1.Function == 0) {
				break;
			}

			// Check if this is a resolved address
			if (thunkData.u1.Function > expectedBaseAddress) {
				// Convert absolute address back to RVA
				ULONGLONG rva = thunkData.u1.Function - actualBaseAddress;

				// We need to find the import information for this function
				// For a proper implementation, we'd need to search the export tables of loaded DLLs
				// or use the original INT data if available

				// For now, we'll just zero out the high bit to make it look like a regular import
				// This is not correct and is just a placeholder
				thunkData.u1.Function = rva;

				// Update the thunk in the IAT
				memcpy(iatSection.content.data() + currentOffset, &thunkData, sizeof(IMAGE_THUNK_DATA64));
			}
		}
		else {
			IMAGE_THUNK_DATA32 thunkData;
			memcpy(&thunkData, iatSection.content.data() + currentOffset, sizeof(IMAGE_THUNK_DATA32));

			// Check for end of thunks
			if (thunkData.u1.Function == 0) {
				break;
			}

			// Check if this is a resolved address
			if (thunkData.u1.Function > expectedBaseAddress) {
				// Convert absolute address back to RVA
				DWORD rva = thunkData.u1.Function - static_cast<DWORD>(actualBaseAddress);

				// Same placeholder approach as above
				thunkData.u1.Function = rva;

				// Update the thunk in the IAT
				memcpy(iatSection.content.data() + currentOffset, &thunkData, sizeof(IMAGE_THUNK_DATA32));
			}
		}

		// Move to next thunk
		currentOffset += thunkSize;
	}
}

// Helper function to fix the resolved IAT addresses in the dumped file
void FixResolvedImports(PEFile& peFile) {
	// Get import directory
	IMAGE_DATA_DIRECTORY importDir;

	if (peFile.is64Bit) {
		importDir = peFile.ntHeaders64.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
	}
	else {
		importDir = peFile.ntHeaders32.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
	}

	if (importDir.VirtualAddress == 0 || importDir.Size == 0) {
		return;
	}

	// Determine the actual base address of the loaded module in memory
	uintptr_t actualBaseAddress;
	uintptr_t expectedBaseAddress;

	if (peFile.is64Bit) {
		expectedBaseAddress = static_cast<uintptr_t>(peFile.ntHeaders64.OptionalHeader.ImageBase);
	}
	else {
		expectedBaseAddress = static_cast<uintptr_t>(peFile.ntHeaders32.OptionalHeader.ImageBase);
	}

	// This would be the actual base address where the module was loaded
	// For this example, we'll assume it's passed as a parameter or determined elsewhere
	// actualBaseAddress = determineActualBaseAddress();

	// If we're dealing with a module that has been loaded at a different address
	// than its preferred base address, we need to adjust the IAT entries
	if (actualBaseAddress != expectedBaseAddress) {
		// Find each import descriptor
		int importSectionIndex = -1;
		DWORD importOffset = 0;

		for (int i = 0; i < peFile.sections.size(); i++) {
			DWORD sectionStart = peFile.sections[i].header.VirtualAddress;
			DWORD sectionEnd = sectionStart + peFile.sections[i].header.Misc.VirtualSize;

			if (importDir.VirtualAddress >= sectionStart && importDir.VirtualAddress < sectionEnd) {
				importSectionIndex = i;
				importOffset = importDir.VirtualAddress - sectionStart;
				break;
			}
		}

		if (importSectionIndex == -1) {
			return;
		}

		PESection& importSection = peFile.sections[importSectionIndex];
		DWORD currentOffset = importOffset;

		while (currentOffset + sizeof(IMAGE_IMPORT_DESCRIPTOR) <= importSection.content.size()) {
			IMAGE_IMPORT_DESCRIPTOR importDesc;
			memcpy(&importDesc, importSection.content.data() + currentOffset, sizeof(IMAGE_IMPORT_DESCRIPTOR));

			// Check for end of import descriptors
			if (importDesc.FirstThunk == 0 && importDesc.Name == 0) {
				break;
			}

			// Fix the IAT entries for this import descriptor
			FixIATForDescriptor(peFile, importDesc, actualBaseAddress, expectedBaseAddress);

			// Move to next descriptor
			currentOffset += sizeof(IMAGE_IMPORT_DESCRIPTOR);
		}
	}
}




template<size_t SIZE>
struct MemoryBlock {
	BYTE data[SIZE];
};

bool DumpProcess(uintptr_t baseAddress, PEFile& outFile) {
	// Read DOS header
	if (!Drv->ReadMemory_ACE((PVOID)baseAddress, &outFile.dosHeader, sizeof(IMAGE_DOS_HEADER))) {
		std::cerr << "Failed to read DOS header" << std::endl;
		return false;
	}

	if (outFile.dosHeader.e_magic != IMAGE_DOS_SIGNATURE) {
		std::cerr << "Invalid DOS signature" << std::endl;
		return false;
	}

	// Read DOS stub in chunks
	SIZE_T dosStubSize = outFile.dosHeader.e_lfanew - sizeof(IMAGE_DOS_HEADER);
	outFile.dosStub.resize(dosStubSize);

	// Read in 4KB chunks with error handling
	constexpr size_t CHUNK_SIZE = 4096;
	for (size_t offset = 0; offset < dosStubSize; offset += CHUNK_SIZE) {
		size_t currentChunkSize = min(CHUNK_SIZE, dosStubSize - offset);
		std::vector<BYTE> chunkBuffer(currentChunkSize);

		if (!Drv->ReadMemory_ACE((PVOID)(baseAddress + sizeof(IMAGE_DOS_HEADER) + offset),
			chunkBuffer.data(), currentChunkSize)) {
			std::cerr << "Warning: Failed to read DOS stub at offset " << offset << std::endl;
			// Fill with zeros instead of failing
			std::fill(chunkBuffer.begin(), chunkBuffer.end(), 0);
		}

		memcpy(outFile.dosStub.data() + offset, chunkBuffer.data(), currentChunkSize);
	}

	// Read PE headers and determine architecture
	uintptr_t peHeaderAddr = baseAddress + outFile.dosHeader.e_lfanew;
	IMAGE_NT_HEADERS32 ntHeaders32;

	if (!Drv->ReadMemory_ACE(PVOID(peHeaderAddr), &ntHeaders32, sizeof(IMAGE_NT_HEADERS32))) {
		std::cerr << "Failed to read NT headers" << std::endl;
		return false;
	}

	if (ntHeaders32.Signature != IMAGE_NT_SIGNATURE) {
		std::cerr << "Invalid NT signature" << std::endl;
		return false;
	}

	outFile.is64Bit = ntHeaders32.OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC;

	if (outFile.is64Bit) {
		if (!Drv->ReadMemory_ACE(PVOID(peHeaderAddr), &outFile.ntHeaders64, sizeof(IMAGE_NT_HEADERS64))) {
			std::cerr << "Failed to read NT headers 64-bit" << std::endl;
			return false;
		}
	}
	else {
		outFile.ntHeaders32 = ntHeaders32;
	}

	// Read sections
	WORD numberOfSections = outFile.is64Bit ?
		outFile.ntHeaders64.FileHeader.NumberOfSections :
		outFile.ntHeaders32.FileHeader.NumberOfSections;

	// Sanity check to prevent excessive memory allocation
	if (numberOfSections > 96) {
		std::cerr << "Suspicious number of sections: " << numberOfSections << std::endl;
		return false;
	}

	uintptr_t sectionHeaderAddr = peHeaderAddr +
		(outFile.is64Bit ?
			sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER) + outFile.ntHeaders64.FileHeader.SizeOfOptionalHeader :
			sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER) + outFile.ntHeaders32.FileHeader.SizeOfOptionalHeader);

	outFile.sections.resize(numberOfSections);

	// Get section alignment for proper handling
	DWORD sectionAlignment = outFile.is64Bit ?
		outFile.ntHeaders64.OptionalHeader.SectionAlignment :
		outFile.ntHeaders32.OptionalHeader.SectionAlignment;

	// Get image base for proper virtual address translation
	uintptr_t imageBase = outFile.is64Bit ?
		static_cast<uintptr_t>(outFile.ntHeaders64.OptionalHeader.ImageBase) :
		static_cast<uintptr_t>(outFile.ntHeaders32.OptionalHeader.ImageBase);

	for (int i = 0; i < numberOfSections; i++) {
		// Read section header
		if (!Drv->ReadMemory_ACE(PVOID(sectionHeaderAddr + i * sizeof(IMAGE_SECTION_HEADER)),
			&outFile.sections[i].header, sizeof(IMAGE_SECTION_HEADER))) {
			std::cerr << "Failed to read section header " << i << std::endl;
			continue;
		}

		// Store original size for reference
		outFile.sections[i].initialSize = outFile.sections[i].header.Misc.VirtualSize;

		// Sanity check on section size
		if (outFile.sections[i].header.Misc.VirtualSize > 0x10000000) {  // 256MB max
			std::cerr << "Section " << i << " has suspicious size: " << outFile.sections[i].header.Misc.VirtualSize << std::endl;
			outFile.sections[i].header.Misc.VirtualSize = 0;
			outFile.sections[i].content.clear();
			continue;
		}

		// If the section has no content in memory, skip reading
		if (outFile.sections[i].header.Misc.VirtualSize == 0) {
			outFile.sections[i].content.clear();
			continue;
		}

		// Detect if section is uninitialized data (like .bss)
		bool isUninitializedData = (outFile.sections[i].header.Characteristics & IMAGE_SCN_CNT_UNINITIALIZED_DATA) != 0;

		if (isUninitializedData) {
			// For BSS sections, just create zeroed buffer
			outFile.sections[i].content.resize(outFile.sections[i].header.Misc.VirtualSize, 0);
			continue;
		}

		// Get absolute address of the section in the target process memory
		uintptr_t sectionAddr = baseAddress + outFile.sections[i].header.VirtualAddress;

		// Allocate buffer for section content
		outFile.sections[i].content.resize(outFile.sections[i].header.Misc.VirtualSize);

		// Read section contents in chunks with error handling
		for (size_t offset = 0; offset < outFile.sections[i].header.Misc.VirtualSize; offset += CHUNK_SIZE) {
			size_t currentChunkSize = min(CHUNK_SIZE,
				static_cast<size_t>(outFile.sections[i].header.Misc.VirtualSize) - offset);

			std::vector<BYTE> chunkBuffer(currentChunkSize);
			bool readSuccess = Drv->ReadMemory_ACE(PVOID(sectionAddr + offset), chunkBuffer.data(), currentChunkSize);

			if (!readSuccess) {
				std::cerr << "Warning: Failed to read section " << i << " at offset " << offset << std::endl;
				// Fill with zeros for unreadable memory
				std::fill(chunkBuffer.begin(), chunkBuffer.end(), 0);
			}

			memcpy(outFile.sections[i].content.data() + offset, chunkBuffer.data(), currentChunkSize);
		}
	}

	// Fix section headers for file output
	DWORD fileAlignment = outFile.is64Bit ?
		outFile.ntHeaders64.OptionalHeader.FileAlignment :
		outFile.ntHeaders32.OptionalHeader.FileAlignment;

	// Calculate and set proper PointerToRawData values
	DWORD currentRawOffset = sizeof(IMAGE_DOS_HEADER) + outFile.dosStub.size() +
		(outFile.is64Bit ? sizeof(IMAGE_NT_HEADERS64) : sizeof(IMAGE_NT_HEADERS32)) +
		numberOfSections * sizeof(IMAGE_SECTION_HEADER);

	// Align to file alignment
	currentRawOffset = (currentRawOffset + fileAlignment - 1) & ~(fileAlignment - 1);

	for (auto& section : outFile.sections) {
		// Set proper raw data size and pointer
		section.header.PointerToRawData = currentRawOffset;
		section.header.SizeOfRawData = (section.content.size() + fileAlignment - 1) & ~(fileAlignment - 1);

		// Update for next section
		currentRawOffset += section.header.SizeOfRawData;
	}

	outFile.AlignSectionHeaders();
	outFile.FixPEHeader();

	// Fix imports and other directories if needed
	FixImportDirectory(outFile);

	return true;
}

#include <fstream>
bool SavePEFile(const PEFile& peFile, const std::string& filename) {
	std::ofstream file(filename, std::ios::binary);
	if (!file) {
		std::cerr << "Failed to create output file" << std::endl;
		return false;
	}

	// Write DOS Header
	file.write(reinterpret_cast<const char*>(&peFile.dosHeader), sizeof(IMAGE_DOS_HEADER));

	// Write DOS Stub
	if (!peFile.dosStub.empty()) {
		file.write(reinterpret_cast<const char*>(peFile.dosStub.data()), peFile.dosStub.size());
	}

	// Write PE Header
	if (peFile.is64Bit) {
		file.write(reinterpret_cast<const char*>(&peFile.ntHeaders64), sizeof(IMAGE_NT_HEADERS64));
	}
	else {
		file.write(reinterpret_cast<const char*>(&peFile.ntHeaders32), sizeof(IMAGE_NT_HEADERS32));
	}

	// Write Section Headers
	for (const auto& section : peFile.sections) {
		file.write(reinterpret_cast<const char*>(&section.header), sizeof(IMAGE_SECTION_HEADER));
	}

	// Calculate file alignment
	DWORD fileAlignment = peFile.is64Bit ?
		peFile.ntHeaders64.OptionalHeader.FileAlignment :
		peFile.ntHeaders32.OptionalHeader.FileAlignment;

	// Align to the start position of the first section
	DWORD currentPos = static_cast<DWORD>(file.tellp());
	DWORD firstSectionPos = peFile.sections.empty() ? currentPos :
		peFile.sections[0].header.PointerToRawData;

	if (firstSectionPos > currentPos) {
		std::vector<char> padding(firstSectionPos - currentPos, 0);
		file.write(padding.data(), padding.size());
	}

	// Write Section Data with proper alignment
	for (size_t i = 0; i < peFile.sections.size(); i++) {
		const auto& section = peFile.sections[i];

		// Make sure we're at the right position
		DWORD expectedPos = section.header.PointerToRawData;
		DWORD currentPos = static_cast<DWORD>(file.tellp());

		if (currentPos < expectedPos) {
			// Add padding to reach the expected position
			std::vector<char> padding(expectedPos - currentPos, 0);
			file.write(padding.data(), padding.size());
		}
		else if (currentPos > expectedPos) {
			// This should not happen if headers are correct
			std::cerr << "Warning: Section position mismatch for section " << i << std::endl;
			// Seek to the right position
			file.seekp(expectedPos);
		}

		// Write section content
		if (!section.content.empty()) {
			file.write(reinterpret_cast<const char*>(section.content.data()), section.content.size());
		}

		// Pad the section to SizeOfRawData
		if (section.header.SizeOfRawData > section.content.size()) {
			DWORD paddingSize = section.header.SizeOfRawData - section.content.size();
			std::vector<char> padding(paddingSize, 0);
			file.write(padding.data(), paddingSize);
		}
	}

	file.close();
	std::cout << "Saved PE file to: " << filename << std::endl;
	return true;
}

void dumpprocessexe(uintptr_t game_base)
{
	// dumper saving to disk
	PEFile peFile;
	if (DumpProcess(game_base, peFile)) {

		if (SavePEFile(peFile, "dumped.exe")) {
			std::cout << "Dump saved successfully!" << std::endl;
		}
		else {
			std::cerr << "Failed to save dump!" << std::endl;
		}
	}
	else
	{
		std::cout << "Failed to dump this nigger  " << GetLastError() << std::endl;
	}

	system("PAUSE");
}
*/

struct ARMOR_INFO
{
	int HelmetLevel;
	float HelmetDurability;
	float HelmetMaxDurability;

	int BreastPlateLevel;
	float BreastDurability;
	float BreastMaxDurability;
};

struct FEquipmentInfo
{
	uintptr_t                                     ItemID;                                                       // 0x0000(0x0008)
	uintptr_t                                     gid;                                                          // 0x0008(0x0008)
	float                                         Health;                                                       // 0x0010(0x0004)
	float                                         MaxHealth;                                                    // 0x0014(0x0004)
	float                                         Durability;                                                   // 0x0018(0x0004)
	float                                         MaxDurability;                                                // 0x001C(0x0004)
	float                                         TotalEquipSeceonds;                                           // 0x0020(0x0004)
	float                                         LastEquipTimeSeconds;                                         // 0x0024(0x0004)
	float                                         TotalApplyDamage;                                             // 0x0028(0x0004)
	char                                          pa_002C[0x0004];                                               // 0x002C(0x0004)
};

template<class T>
struct TArray
{
	friend class FString;

public:
	TArray()
	{
		Data = nullptr;
		Count = Max = 0;
	}

	TArray(T* data, int32_t count, int32_t max)
		: Data(data),
		Count(count),
		Max(max)
	{
	}

	int Num() const
	{
		return Count;
	}

	T& operator[](int i)
	{
		return Data[i];
	}

	const T& operator[](int i) const
	{
		return Data[i];
	}

	bool IsValidIndex(int i) const
	{
		return i < Num();
	}


	T* Data;
	int32_t Count;
	int32_t Max;

};

int GetArmorLevel(int ItemID)
{
	// Level 1
	if ((ItemID >= -1834900887 && ItemID <= -1834900881) || (ItemID >= -1874900889 && ItemID <= -1874900880)) {
		return 1;
	}

	// Level 2
	if ((ItemID >= -1834899887 && ItemID <= -1834899881) || (ItemID >= -1874899889 && ItemID <= -1874899880)) {
		return 2;
	}

	// Level 3
	if ((ItemID >= -1834898887 && ItemID <= -1834898881) || (ItemID >= -1874898889 && ItemID <= -1874898880)) {
		return 3;
	}

	// Level 4
	if ((ItemID >= -1834897887 && ItemID <= -1834897881) || (ItemID >= -1874897889 && ItemID <= -1874897880)) {
		return 4;
	}

	// Level 5
	if ((ItemID >= -1834896887 && ItemID <= -1834896881) || (ItemID >= -1874896889 && ItemID <= -1874896880)) {
		return 5;
	}

	// Level 6
	if ((ItemID >= -1834895887 && ItemID <= -1834895881) || (ItemID >= -1874895889 && ItemID <= -1874895880)) {
		return 6;
	}

	// Default case (if no level matches)
	return -1; // Return -1 if ItemID doesn't belong to any known level
}
enum class EAICharacterTag : uint8_t
{
	EAICharacterTag_None = 0,
	EAICharacterTag_Elite = 1,
	EAICharacterTag_Civilian = 2,
	EAICharacterTag_Sniper = 3,
	EAICharacterTag_Shield = 4,
	EAICharacterTag_HeavyArmor = 5,
	EAICharacterTag_Paratrooper = 6,
	EAICharacterTag_Melee = 7,
	EAICharacterTag_Firebat = 8,
	EAICharacterTag_RPG = 9,
	EAICharacterTag_Boss = 10,
	EAICharacterTag_BoyBand = 11,
	EAICharacterTag_Crocodile = 12,
	EAICharacterTag_NonHuman = 13,
	EAICharacterTag_HelicopterPilot = 14,
	EAICharacterTag_MAX = 15

};

std::string GetTagString(EAICharacterTag tag) {

	switch (tag) {
	case EAICharacterTag::EAICharacterTag_Elite: return ("Elite");
	case EAICharacterTag::EAICharacterTag_Civilian: return ("Civilian");
	case EAICharacterTag::EAICharacterTag_Sniper: return ("Sniper");
	case EAICharacterTag::EAICharacterTag_Shield: return ("Shield");
	case EAICharacterTag::EAICharacterTag_HeavyArmor: return ("Heavy");
	case EAICharacterTag::EAICharacterTag_Paratrooper: return ("Para");
	case EAICharacterTag::EAICharacterTag_Melee: return ("Melee");
	case EAICharacterTag::EAICharacterTag_Firebat: return ("Fire");
	case EAICharacterTag::EAICharacterTag_RPG: return ("RPG");
	case EAICharacterTag::EAICharacterTag_Boss: return ("Boss");
	case EAICharacterTag::EAICharacterTag_BoyBand: return ("Band");
	case EAICharacterTag::EAICharacterTag_Crocodile: return ("Croc");
	case EAICharacterTag::EAICharacterTag_NonHuman: return ("NonHuman");
	default: return ("Unknown");
	}
}

ImColor GetArmorColor(int level) {
	const ImColor armorColors[] = {
		ImColor(255, 255, 255, 255),  // Level 1: White
		ImColor(0, 128, 0, 255),      // Level 2: Dark Green
		ImColor(100, 170, 255, 255),   // Level 3: light Blue
		ImColor(223,41, 223, 255),    // Level 4: Purple
		ImColor(255, 223, 0, 255),    // Level 5: Golden
		ImColor(180, 0, 0, 255)       // Level 6: Dark Red
	};

	if (level >= 1 && level <= 6) {
		return armorColors[level - 1];
	}

	// Default to white if the level is invalid
	return ImColor(255, 255, 255, 255);
}

ImColor GetTeamColor(int teamId) {
	static std::unordered_map<int, ImColor> teamColors;

	// Special case: teamId 0 gets white
	if (teamId == 0) {
		return ImColor(255, 255, 255);
	}

	// If color for the teamId is already generated, return it
	if (teamColors.find(teamId) != teamColors.end()) {
		return teamColors[teamId];
	}

	// Generate a deterministic color based on teamId
	int r = (teamId * 53) % 256;
	int g = (teamId * 97) % 256;
	int b = (teamId * 193) % 256;

	ImColor color(r, g, b);

	// Store the generated color
	teamColors[teamId] = color;

	return color;
}

struct FString
{
	TArray<TCHAR> Data;
};

inline std::string ReadFString(uintptr_t address) {
	// Read FString structure
	FString fstring_struct = read<FString>(address);

	// Validate pointer and count
	if (!fstring_struct.Data.Data || fstring_struct.Data.Num() <= 0)
		return "";

	// Read the actual string data
	std::wstring player_name(fstring_struct.Data.Num(), L'\0');
	ReadMemory((void*)fstring_struct.Data.Data, player_name.data(), fstring_struct.Data.Num() * sizeof(wchar_t));

	// Convert and return as std::string
	return std::string(player_name.begin(), player_name.end());
}

std::wstring ReadFStringW(uintptr_t address) {
	// Read FString structure
	FString fstring_struct = read<FString>(address);

	// Validate pointer and count
	if (!fstring_struct.Data.Data || fstring_struct.Data.Num() <= 0)
		return L"";

	// Read the actual string data
	std::wstring player_name(fstring_struct.Data.Num(), L'\0');
	ReadMemory((void*)fstring_struct.Data.Data, player_name.data(), fstring_struct.Data.Num() * sizeof(wchar_t));

	return player_name;
}

std::string WideToUTF8(const std::wstring& wstr) {
	if (wstr.empty()) return "";

	int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
	std::string str(size_needed - 1, 0);
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &str[0], size_needed, nullptr, nullptr);

	return str;
}

std::string ToString(EMarkingItemType type) {
	switch (type) {
	case EMarkingItemType::None: return "None";
	case EMarkingItemType::CommonItem: return "CommonItem";
	case EMarkingItemType::LootingItem: return "LootingItem";
	case EMarkingItemType::Container: return "Container";
	case EMarkingItemType::DyingTeammate: return "DyingTeammate";
	case EMarkingItemType::DeadBody: return "DeadBody";
	case EMarkingItemType::Weapon: return "Weapon";
	case EMarkingItemType::EngineerSonicTrap: return "EngineerSonicTrap";
	case EMarkingItemType::Vehicle: return "Vehicle";
	case EMarkingItemType::TacticalEquip: return "TacticalEquip";
	case EMarkingItemType::PendingDeathCharacter: return "PendingDeathCharacter";
	case EMarkingItemType::SectorAnchor: return "SectorAnchor";
	case EMarkingItemType::MandelBrickInteractor: return "MandelBrickInteractor";
	case EMarkingItemType::MandelBrickDecipherDevice: return "MandelBrickDecipherDevice";
	case EMarkingItemType::COUNT: return "COUNT";
	case EMarkingItemType::EMarkingItemType_MAX: return "EMarkingItemType_MAX";
	default: return "Unknown";
	}
}

BYTE GetXorKey(size_t length) {
	constexpr uint32_t MAGIC = 0x38E38E39;
	const uint32_t r9d = static_cast<uint32_t>(length);
	uint64_t temp = (static_cast<uint64_t>(MAGIC) * r9d) >> 32;
	temp >>= 1;
	uint32_t remainder = r9d - (temp * 9);

	if (remainder > 8) return 0;

	switch (remainder) {
	case 0: { uint8_t key = static_cast<uint8_t>(r9d); key = (key & 0x1F) + 0x80; key += static_cast<uint8_t>(r9d); key |= 0x7F; return key; }
	case 1: { uint8_t key = static_cast<uint8_t>(r9d) ^ 0xDF; key += 0x80; key += static_cast<uint8_t>(r9d); key |= 0x7F; return key; }
	case 2: { uint8_t key = static_cast<uint8_t>(r9d) | 0xCF; key += 0x80; key += static_cast<uint8_t>(r9d); key |= 0x7F; return key; }
	case 3: { int8_t signed_len = static_cast<int8_t>(r9d); uint8_t key = static_cast<uint8_t>(signed_len * 0x21); key += 0x80; key |= 0x7F; return key; }
	case 4: { uint8_t key = (static_cast<uint8_t>(r9d) >> 2); key += 0x80; key += static_cast<uint8_t>(r9d); key |= 0x7F; return key; }
	case 5: { uint8_t key = static_cast<uint8_t>(r9d - 0x29); key += key; key += static_cast<uint8_t>(r9d - 0x29); key |= 0x7F; return key; }
	case 6: { uint8_t key = (static_cast<uint8_t>(r9d) << 2) | 0x05; key += 0x80; key += static_cast<uint8_t>(r9d); key |= 0x7F; return key; }
	case 7: { uint8_t key = (static_cast<uint8_t>(r9d) >> 4) | 0x07; key += 0x80; key += static_cast<uint8_t>(r9d); key |= 0x7F; return key; }
	case 8: { uint8_t key = static_cast<uint8_t>(r9d) ^ 0x40; key += 0x80; key += static_cast<uint8_t>(r9d); key |= 0x7F; return key; }
	}
	return 0;
}

std::string get_fname(int key) {
	static std::unordered_map<int, std::string> fname_map;
	auto it = fname_map.find(key);
	if (it != fname_map.end()) return it->second;

	unsigned int chunkOffset = (unsigned int)((int)(key) >> 18);
	unsigned int nameOffset = (unsigned int)(0x3FFFF & key);
	std::uint64_t namePoolChunk = read<std::uint64_t>((std::uintptr_t)(process_base + 0x1435E940 + ((chunkOffset + 1) * 8)));
	std::uint64_t entryOffset = namePoolChunk + (2 * nameOffset);

	unsigned short nameEntry = read<unsigned short>(entryOffset);
	auto name_size = nameEntry >> 6;
	if (name_size <= 64 && name_size > 0) {
		char AnsiName[64];
		memset(AnsiName, 0, sizeof(AnsiName));
		if (ReadMemory((PVOID)(entryOffset + 2), AnsiName, name_size)) {
			for (std::uint16_t i = 0; i < name_size; i++) {
				AnsiName[i] = AnsiName[i] ^ GetXorKey(name_size);
			}
			char* pos = strrchr(AnsiName, '_');
			if (pos && *(pos + 1) == 'C') {
				*(pos + 2) = '\0';
			}
			if (strcmp(AnsiName, ""))
				fname_map.try_emplace(key, std::string(AnsiName));
			return AnsiName;
		}
	}
	return std::string{ "" };
}
