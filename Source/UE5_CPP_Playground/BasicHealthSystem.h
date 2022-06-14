// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BasicHealthSystem.generated.h"


UENUM()
enum class EHealthTypeModifier : uint8 { Add, Subtract };
UENUM()
enum class EDeathType : uint8 {	Player,	Enemy, NPC };
UENUM()
enum class EDeathActions : uint8 {	Respawn, Restart, Pause, Menu };


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE5_CPP_PLAYGROUND_API UBasicHealthSystem : public UActorComponent
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDealDamageEvent, float, Amount);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHandleDeathEvent, EDeathActions, OnDeathActions);
	

public:	
	// Sets default values for this component's properties
	UBasicHealthSystem();
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintAssignable, Category="Basic Health Damage", meta=(AllowPrivateAccess))
	FOnDealDamageEvent OnDealDamage;
	
	UPROPERTY(BlueprintAssignable, Category="Basic Health Damage", meta=(AllowPrivateAccess))
	FOnHandleDeathEvent OnHandleDeath;

	/**
	 * @brief Componente usado para tocar o audio de cura e hit 
	 */
	class UAudioComponent* HealthAudioComponent = nullptr;
	/**
	 * @brief Componente usado para tocar o audio do Shield 
	 */
	class UAudioComponent* ShieldAudioComponent = nullptr;
	/**
	 * @brief Componente usado para tocar o audio de Stamina 
	 */
	class UAudioComponent* StaminaAudioComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Health)
	bool bIsAlive = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Health)
	float CurrentHealth = 0.f;
	/**
	 * @brief Max Health also set the amount to actor's current health in begin play 
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Health, meta=(UIMin = 0.0, UIMax = 100))
	float MaxHealth = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Health, meta=(UIMin = 0.0, UIMax = 10.0))
	float OvertimeIncrease = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Health, meta=(UIMin = 0.0, UIMax = 10.0))
	float OvertimeDecrease = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Health, meta=(UIMin = 0.0, UIMax = 1.0))
	float IncreaseRate = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Health, meta=(UIMin = 0.0, UIMax = 1.0))
	float DecreaseRate = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Health)
	bool bRecoveryHealth = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Health)
	bool bDecreaseHealth = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Sounds)
	bool bPlayHealthCureSound = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Sounds)
	bool bPlayHealthHitSound = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Sounds)
	bool bPlayShieldHitSound = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Sounds)
	bool bPlayHealthLowSound = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Sounds)
	bool bPlayShieldLowSound = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Sounds)
	bool bPlayDeathSound = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Sounds)
	bool bPlayShieldBreakSound = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Sounds)
	bool bPlayStaminaLowSound = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Sounds)
	class USoundBase* HealthCureSound = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Sounds)
	class USoundBase* HealthHitSound = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Sounds)
	class USoundBase* ShieldHitSound = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Sounds)
	class USoundBase* HealthLowSound = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Sounds)
	class USoundBase* ShieldLowSound = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Sounds)
	class USoundBase* DeathSound = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Sounds)
	class USoundBase* ShieldBreakSound = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Sounds)
	class USoundBase* StaminaLowSound = nullptr;

public:

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(EditAnywhere, Category=Death)
	EDeathType ActorType = EDeathType::Player;
	
	UPROPERTY(EditAnywhere, Category=Death);
	EDeathActions OnDeath = EDeathActions::Respawn;

	UFUNCTION(BlueprintCallable)
	void SetLowAudioComponents(UAudioComponent* HealthAudioComponentRef, UAudioComponent* ShieldAudioComponentRef, UAudioComponent* StaminaAudioComponentRef);
	
	UFUNCTION(BlueprintCallable)
	void ModifyHealth(EHealthTypeModifier ChangeType, float Amount, bool bDebug = false);

	UFUNCTION(BlueprintCallable)
	void KillActor(EDeathType ActorTypeRef, EDeathActions OnDeathAction, float Delay, bool bPlaySound);
	
	UFUNCTION(BlueprintCallable)
	void DealDamage(float Amount);
	
	UFUNCTION(BlueprintCallable)
	void UpdateUI();

	void IncreaseHealthOverTime(float Amount, float Rate, float Duration);
	
	void DecreaseHealthOverTime(float Amount, float Rate, float Duration);

	// setget bIsAlive
	bool GetIsAlive() const { return bIsAlive; }
	
	void SetIsAlive(const bool bNewCondition) { bIsAlive = bNewCondition; }

	// setget CurrentHealth
	int GetCurrentHealth() const { return CurrentHealth; }
	
	void SetCurrentHealth(const float Amount) { CurrentHealth = Amount; }

	// setget Recovery Health
	void SetRecoveryHealth(bool bNewCondition) { bRecoveryHealth = bNewCondition; }
	
	bool GetRecoveryHealth() { return bRecoveryHealth; }

	// setget Decrease Health
	void SetDecreaseHealth(bool bNewCondition) { bDecreaseHealth = bNewCondition; }
	
	bool GetDecreaseHealth() { return bDecreaseHealth; }

};

