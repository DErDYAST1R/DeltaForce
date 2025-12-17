#pragma once

namespace Offsets {
    uintptr_t localplayerscan;
    uintptr_t BitTest1scan;

    uintptr_t GWorld = 0x13C187A8;
    uintptr_t GNames = 0x14416AC0;
    uintptr_t ObjectIDName = 0x1C; //Off::UObject::Name

    uint32_t PersistentLevel = 0xF8;               //class Level PersistentLevel;                             // Class Engine.World
    uint32_t LevelCollections = 0x168;
    uint32_t OwningGameInstance = 0x190;            //class GameInstance OwningGameInstance;                   // Class Engine.World

    uint32_t MaxPacket = 0x98 + 0x8;                      //int32_t MaxPacket;                                       // Class Engine.NetConnection
    uint32_t OwningActor = 0x98;                    //struct AActor* OwningActor;                              // Class Engine.NetConnection //actors_offset

    uint32_t LocalPlayers = 0x38;                   //struct TArray<class struct LocalPlayer*> LocalPlayers;   // Class Engine.GameInstance

    uint32_t PlayerController = 0x30;               //class PlayerController PlayerController;                 // Class Engine.Player

    uint32_t PlayerCameraManager = 0x408;           //class PlayerCameraManager PlayerCameraManager;           // Class Engine.PlayerController
    uint32_t AcknowledgedPawn = 0x3F0;              //class Pawn AcknowledgedPawn;                             // Class Engine.PlayerController

    uint32_t Mesh = 0x3D0;                          //class SkeletalMeshComponent Mesh;                        // Class Engine.Character

    uint32_t PlayerState = 0x390;                   //class PlayerState PlayerState;                           // Class Engine.Pawn //APawn : public AActor

    uint32_t LastSubmitTime = 0x2F4 + 0x4;          //float BoundsScale; + 0x4                                 // Class Engine.PlayerState
    uint32_t LastRenderTime = LastSubmitTime + 0x8;

    uint32_t StaticMesh = 0x728; // bonearray //Class Engine.StaticMeshComponent //class StaticMeshComponent : MeshComponent

    uint32_t ComponentToWorld = 0x210; //absolute_position_offset

    uint32_t PlayerNamePrivate = 0x478;             //struct FString PlayerNamePrivate; // Class Engine.PlayerState

    uint32_t bDead = 0x4B4; //Class GPGameplay.GPPlayerState
    uint32_t bFinishGame = 0x4C8; //Class GPGameplay.GPPlayerState
    uint32_t TeamId = 0x66C; //Class GPGameplay.GPPlayerState
    uint32_t Camp = 0x670; ////Class GPGameplay.GPPlayerState

    uint32_t bIsABot = 0x39E; //AplayerState //Class Engine.PlayerState

    uint32_t CharacterTags = 0x34F0; //ADFMCharacter //TArray<EAICharacterTag> CharacterTags //ADFMAICharacter : public ADFMCharacter

    uint32_t Instigator = 0x168; // //Actors - Instigator //0x170 //struct Pawn* Instigator;

    uint32_t Viewtarget = 0x2DEC0; // AplayerCameraManager //APlayerCameraManager : public AActor //struct FTViewTarget ViewTarget;

    uint32_t Password = 0xDCC;//class AInteractor_HackPC final : public AInteractorBaseProxy //Class DFMGameplay.Interactor_HackPC
    uint32_t MarkingItemType = 0x71A; //class AInteractorBase : public AGPInteractorBase //Class DFMGameplay.InteractorBase

    uint32_t CharacterMovement = 0x3D8; //class alignas(0x10) ACHARACTER : public APawn // Class Engine.CHARACTER

    uint32_t LastUpdateVelocity = 0x2AC; //class UCharacterMovementComponent : public UPawnMovementComponent / Class Engine.CharacterMovementComponent
    uint32_t MaxFlySpeed = 0x1E8; //class UCharacterMovementComponent : public UPawnMovementComponent / Class Engine.CharacterMovementComponent

    uint32_t CharacterEquipComponent = 0x30B8; // DFMGameplay.DFMPlayerCharacter // class CharacterEquipComponent CharacterEquipComponent; // 0x2d50(0x8)
    uint32_t EquipmentInfoArray = 0x1D8; // DFMGameplay.CharacterEquipComponent //TArray<struct FEquipmentInfo> EquipmentInfoArray;

    uint32_t ControlRotation = 0x3D8; //AplayerController

    uint32_t HealthComp = 0xF68; //ACharacterBase //class UGPHealthDataComponent* HealthComp

    uint32_t HealthSet = 0x248; //GhealthDataComponenet //class UGPAttributeSetHealth* HealthSet;
    uint32_t CurrentValue = 0xC; //GAttributeData

    uint32_t Health = 0x30; // GAttributeSetHealth
    uint32_t MaxHealth = 0x48; // GAttributeSetHealth
    uint32_t ArmorHealth = 0x68; // GAttributeSetHealth
    uint32_t MaxArmorHealth = 0x80; // GAttributeSetHealth

    uint32_t UNetDriver = 0x30; // Engine.NetDriver //UNetDriver

    uint32_t ActorsList = 0x570; // Engine.NetDriver //UNetDriver

    uint32_t RootComponent = 0x180; //Engine.Actor

    uint32_t RelativeLocation = 0x0168; //USceneComponent

    uint32_t AttachInfo = 0xED8; // DFMGameplay.InventoryPickup //AInventoryPickUp
    uint32_t CommonItemRow = AttachInfo - 0x8; // Undocumented on dumpers for some reason 

    // DFMGlobalDefines.DFMCommonItemRow
    namespace FDFMCommonItemRow {
        uint32_t Quality = 0x68;
        uint32_t GuidePrice = 0xd8; //InitialGuidePrice
    }
};

void GArenaOffsets()
{
    Offsets::GWorld = 0x13A98518;
    Offsets::GNames = 0x1442D5C0;

    Offsets::PersistentLevel = 0xF8;
    Offsets::LevelCollections = 0x168;
    Offsets::OwningGameInstance = 0x190;
    //Offsets::MaxPacket = 0xB0;
    //Offsets::OwningActor = 0xA8;
    Offsets::LocalPlayers = 0x38;
    Offsets::PlayerController = 0x30;
    Offsets::PlayerCameraManager = 0x408;
    Offsets::AcknowledgedPawn = 0x3F0;
    Offsets::Mesh = 0x3D0;
    Offsets::PlayerState = 0x0390;
    Offsets::LastSubmitTime = 0x02F4 + 0x4;
    Offsets::LastRenderTime = Offsets::LastSubmitTime + 0x8;
    Offsets::StaticMesh = 0x728;
    Offsets::PlayerNamePrivate = 0x478;
    Offsets::bDead = 0x04B4;
    Offsets::bFinishGame = 0x04C8;
    Offsets::TeamId = 0x066C;
    Offsets::Camp = 0x0670;
    Offsets::bIsABot = 0x039E;
    Offsets::CharacterTags = 0x34F0;
    Offsets::Instigator = 0x168;
    Offsets::Viewtarget = 0x2DEC0;
    Offsets::Password = 0xDCC;
    Offsets::MarkingItemType = 0x71A;
    Offsets::CharacterMovement = 0x3D8;
    Offsets::LastUpdateVelocity = 0x2AC;
    Offsets::MaxFlySpeed = 0x1E8;
    Offsets::CharacterEquipComponent = 0x30B8;
    Offsets::EquipmentInfoArray = 0x1D8;
    Offsets::ControlRotation = 0x3D8;
    Offsets::HealthComp = 0xF68;
    Offsets::HealthSet = 0x248;
    Offsets::CurrentValue = 0xC;
    Offsets::Health = 0x30;
    Offsets::MaxHealth = 0x48;
    Offsets::ArmorHealth = 0x68;
    Offsets::MaxArmorHealth = 0x80;
    Offsets::UNetDriver = 0x30;
    Offsets::RootComponent = 0x180;
    Offsets::RelativeLocation = 0x168;
    Offsets::AttachInfo = 0xED8;
    Offsets::CommonItemRow = Offsets::AttachInfo - 0x8;

}

void WeGameOffsets()
{
    Offsets::GWorld = 0x138A5CC8; // auto update by pattern
    Offsets::GNames = 0x14225a40; // auto update by pattern 

    //Offsets::OwningActor = 0xa8;
    //Offsets::MaxPacket = 0x00B0;

    Offsets::PersistentLevel = 0x158;

    Offsets::LocalPlayers = 0x0038;
    Offsets::PlayerController = 0x30;

    Offsets::Mesh = 0x03D0;
    Offsets::PlayerState = 0x0390;

    Offsets::LastSubmitTime = 0x02F4 + 0x4;

    Offsets::StaticMesh = 0x0728;

    Offsets::PlayerNamePrivate = 0x0478;

    Offsets::bDead = 0x04B4;
    Offsets::bFinishGame = 0x04C8;
    Offsets::TeamId = 0x066C;
    Offsets::Camp = 0x0670;
    Offsets::bIsABot = 0x039E;

    Offsets::CharacterTags = 0x34F0;
    Offsets::Instigator = 0x0168;

    Offsets::Viewtarget = 0x2DEC0;

    Offsets::CharacterEquipComponent = 0x30B8;
    Offsets::EquipmentInfoArray = 0x01D8;
    Offsets::ControlRotation = 0x03D8;

    Offsets::HealthComp = 0x0F68;
    Offsets::HealthSet = 0x0248;
    Offsets::CurrentValue = 0x000C;
    Offsets::Health = 0x0030;
    Offsets::MaxHealth = 0x0048;
    Offsets::ArmorHealth = 0x0068;
    Offsets::MaxArmorHealth = 0x0080;

    Offsets::UNetDriver = 0x0030;

    Offsets::RootComponent = 0x0180;
    Offsets::RelativeLocation = 0x0168;
    Offsets::AttachInfo = 0x0ED8;

}

bool wegame;

void get_gamemode()
{
    HWND steam = FindWindowA("UnrealWindow", "Delta Force  ");
    HWND Garena = FindWindowA("UnrealWindow", "Garena Delta Force  ");
    HWND WeGame = FindWindowW(L"UnrealWindow", L"三角洲行动  ");

    if (steam) {
        printf("Steam\n");
        return;
    }
    else if (Garena)
    {
        GArenaOffsets();
        printf("Garena\n");

        return;
    }
    else if (WeGame) {
        WeGameOffsets();
        wegame = true;
        printf("Wegame\n");
        return;
    }
    //printf("ERROR!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
}