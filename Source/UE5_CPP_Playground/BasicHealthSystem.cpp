// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicHealthSystem.h"
#include <algorithm>
#include "Components/AudioComponent.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values for this component's properties
UBasicHealthSystem::UBasicHealthSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	
}

void UBasicHealthSystem::BeginPlay()
{
	Super::BeginPlay();

	SetCurrentHealth(MaxHealth);
}


void UBasicHealthSystem::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// CHECK HEALTH AND IS ALIVE
	if ((GetCurrentHealth() <= 0) && GetIsAlive())
	{
		// DEBUG LOG
		UE_LOG(LogTemp, Warning, TEXT("MORREU PO"));

		// TRIGGER DEATH ACTIONS
		KillActor(ActorType, OnDeath, 0.5f, true);
	}

	if (bRecoveryHealth)
	{
		IncreaseHealthOverTime(OvertimeIncrease, IncreaseRate, 1.0);
	}
	
}

/**
 * @brief Set Audio Components to play audio functions
 * @param HealthAudioComponentRef Hit and Cure audio component
 * @param ShieldAudioComponentRef Shield audio component
 * @param StaminaAudioComponentRef Stamina audio component
 */
void UBasicHealthSystem::SetLowAudioComponents(UAudioComponent* HealthAudioComponentRef,
                                               UAudioComponent* ShieldAudioComponentRef,
                                               UAudioComponent* StaminaAudioComponentRef)
{
	HealthAudioComponent = HealthAudioComponentRef;
	HealthAudioComponent->SetSound(HealthHitSound);
	
	ShieldAudioComponent = ShieldAudioComponentRef;
	ShieldAudioComponent->SetSound(ShieldHitSound);
	
	StaminaAudioComponent = StaminaAudioComponentRef;
	StaminaAudioComponent->SetSound(StaminaLowSound);
}

void UBasicHealthSystem::ModifyHealth(const EHealthTypeModifier ChangeType, const float Amount, const bool bDebug)
{

	switch (ChangeType)
	{
		case EHealthTypeModifier::Add:

			if (bPlayHealthCureSound)
			{
				// play audio
				HealthAudioComponent->SetSound(HealthCureSound);
				HealthAudioComponent->Play();

				// TODO Particles and VFX

				// TODO Text info with Hit Amount
			}
			
			CurrentHealth = std::min((CurrentHealth + Amount), MaxHealth);

			if (bDebug)
			{
				UE_LOG(LogTemp, Warning, TEXT("Current Health %f"), CurrentHealth);
			}
		
			break;

		case EHealthTypeModifier::Subtract:
			
			if (bPlayHealthHitSound)
			{
				// play audio
				HealthAudioComponent->SetSound(HealthHitSound);
				HealthAudioComponent->Play();

				// TODO Particles and VFX

				// TODO Text info with Hit Amount
				
			}
			
			CurrentHealth = std::max(0.f, (CurrentHealth - Amount));

			if (bDebug)
			{
				UE_LOG(LogTemp, Warning, TEXT("Current Health %f"), CurrentHealth);
			}
		
			break;
				
		default:
			break;
			
	}
	
}

/**
 * @brief Kill Actor and handle to the Death actions
 * @param ActorTypeRef Actor type
 * @param OnDeathAction Death action type
 * @param Delay Time to trigger next action
 * @param bPlaySound If true, play death sound
 */
void UBasicHealthSystem::KillActor(EDeathType ActorTypeRef, EDeathActions OnDeathAction, float Delay, bool bPlaySound)
{

	UE_LOG(LogTemp, Warning, TEXT("TO MORRIDO MESMO SO QUE DENTRO DA FUNCAO SAFE"));
	
	// TODO Death Actions

	OnHandleDeath.Broadcast(OnDeathAction);
	
	switch (ActorTypeRef) {
		case EDeathType::Player: SetIsAlive(false);
		case EDeathType::Enemy: GetOwner()->Destroy();
		case EDeathType::NPC: GetOwner()->Destroy();
		default: ;
	}
}

void UBasicHealthSystem::DealDamage(float Amount)
{
	// CHECK IS ALIVE
	if (!GetIsAlive()) return;

	// CHECK AUDIO COMPONENT
	if (!IsValid(HealthAudioComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("Componente de Audio não foi atribuido"));
		return;
	}

	//PLAY AUDIO
	HealthAudioComponent->SetSound(HealthHitSound);
	HealthAudioComponent->Play();

	// SET NEW HEALTH
	SetCurrentHealth(std::max(0.f, (CurrentHealth - Amount)));

	// DEBUG LOG
	UE_LOG(LogTemp, Warning, TEXT("Current Health %f"), CurrentHealth);

	// DELEGATE EVENT
	OnDealDamage.Broadcast(CurrentHealth);
	
}


/**
 * @brief Update Widget UI with current proprieties
 */
void UBasicHealthSystem::UpdateUI()
{
	// TODO Update UI Function
	// Get Widget and call Update UI Function
}

void UBasicHealthSystem::DecreaseHealthOverTime(float Amount, float Rate, float Duration)
{
	ModifyHealth(EHealthTypeModifier::Subtract, Amount * Rate, true);
}

void UBasicHealthSystem::IncreaseHealthOverTime(float Amount, float Rate, float Duration)
{
	if (Duration > 0)
	{
		ModifyHealth(EHealthTypeModifier::Add, Amount * Rate, true);
		Duration = Duration - FApp::GetDeltaTime();
	}

	return SetRecoveryHealth(false);
	
}

