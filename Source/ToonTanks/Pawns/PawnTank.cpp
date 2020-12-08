// Fill out your copyright notice in the Description page of Project Settings.

#include "PawnTank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Kismet/GameplayStatics.h"

APawnTank::APawnTank()
{
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    SpringArm->SetupAttachment(RootComponent);

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void APawnTank::BeginPlay()
{
    Super::BeginPlay();

    PlayerControllerRef = Cast<APlayerController>(GetController());
    UGameplayStatics::PlaySound2D(this, Wheels);	
}

void APawnTank::HandleDestruction() 
{
    Super::HandleDestruction();
    // Hide Player. TODO - Create new function to handle this. 
    bIsPlayerAlive = false;

    SetActorHiddenInGame(true);
    SetActorTickEnabled(false);
}

bool APawnTank::GetIsPlayerAlive() 
{
    return bIsPlayerAlive;
}

// Called every frame
void APawnTank::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    CapsuleComp->AddForce(GetActorForwardVector() * MoveValue);
    CapsuleComp->AddTorqueInDegrees(GetActorUpVector() * RotateValue);
   
    if(PlayerControllerRef)
    {
        FHitResult TraceHitResult;
        PlayerControllerRef->GetHitResultUnderCursor(ECC_Visibility, false, TraceHitResult);
        FVector HitLocation = TraceHitResult.ImpactPoint;

        RotateTurret(HitLocation);
    }
}

// Called to bind functionality to input
void APawnTank::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
     PlayerInputComponent->BindAxis("MoveForward", this, &APawnTank::CalculateMoveInput);
     PlayerInputComponent->BindAxis("Turn", this, &APawnTank::CalculateRotateInput);
     PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APawnTank::Fire);
 
}

void APawnTank::CalculateMoveInput(float Value) 
{
    if(MayMove)
    {
        MoveValue = Value;
    }
    UE_LOG(LogTemp, Warning, TEXT("%f"), MoveValue);
}

void APawnTank::CalculateRotateInput(float Value) 
{
    if (MayMove)
    {
        RotateValue = Value;
    }
    UE_LOG(LogTemp, Warning, TEXT("%f"), RotateValue);
}

/*
void APawnTank::RotateOwnTurret(HL)
{
    
}
*/
