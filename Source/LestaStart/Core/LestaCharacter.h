//	Describes player character 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputAction.h"
#include "Kismet/GameplayStatics.h"
#include "../ActorInterface.h"
#include "../HealthComponent.h"
#include "../GrenadeShootComponent.h"
#include "../TracePlayersComponent.h"
#include "../LazerShootComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "LestaCharacter.generated.h"

class UCameraComponent;

UENUM()
enum class WEAPON_TYPE {
	LAZER,
	GRENADE,
};

UCLASS()
class LESTASTART_API ALestaCharacter : public ACharacter, public IActorInterface
{
	GENERATED_BODY()

private:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FShootingStatus, bool, Flag);
	UPROPERTY(Replicated)
	FShootingStatus ShootingStatus;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartReload);
	UPROPERTY(Replicated)
	FStartReload StartReload;

	UPROPERTY(Replicated)
	bool IsShooting;

	UPROPERTY(Replicated)
	bool IsReloading;

	UPROPERTY(Replicated)
	WEAPON_TYPE ChoisenWeapon;

	UPROPERTY(VisibleAnywhere, Category = "Player")
		APlayerController* UserPlayerController;

public:
	ALestaCharacter();

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const override;

	UFUNCTION()
		virtual void GetDamage(const double& Damage) override;

	UPROPERTY(EditDefaultsOnly, Replicated)
		TObjectPtr<UHealthComponent> HealthComponent;

	UPROPERTY(EditDefaultsOnly, Replicated)
		TObjectPtr<UGrenadeShootComponent> GrenadeComponent;

	UPROPERTY(EditDefaultsOnly, Replicated)
		TObjectPtr<UTracePlayersComponent> TraceComp;

	UPROPERTY(EditDefaultsOnly, Replicated)
		TObjectPtr<ULazerShootComponent> LazerComponent;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReloadingDelegate, bool, ReloadingFlag);
	FReloadingDelegate Reloading;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChangeHPDelegate, double, HP);
	FChangeHPDelegate ChangeHP;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChangeWeaponDelegate, FString, Weapon);
	FChangeWeaponDelegate ChangeWeapon;

protected:

	UPROPERTY(Replicated)
	double GrenadeTimeCount;

	UFUNCTION(Server, Unreliable)
		void KillPlayer();

	UFUNCTION(NetMulticast,Unreliable)
		void EndOfReload();

	UFUNCTION(Client, Unreliable)
		void ChangeHPHUD(double HP);

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputMappingContext> IMC;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> CameraComponent;

	/** Input action assigned to movement. */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveInputAction;

	/** Input action assigned to camera movement. */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> LookInputAction;

	/** Shoot Input Action **/
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> ShootInputAction;

	/** Change Weapon Input Action **/
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> ChangeWeaponInputAction;

	/** Reload Input Action **/
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> ReloadInputAction;

	virtual void OnShootInput(const FInputActionInstance& InputActionInstance);
	virtual void OnChangeWeaponInput(const FInputActionInstance& InputActionInstance);
	virtual void OnReloadInput(const FInputActionInstance& InputActionInstance);

	virtual void OnMoveInput(const FInputActionInstance& InputActionInstance);
	virtual void OnLookInput(const FInputActionInstance& InputActionInstance);

	virtual void BeginPlay() override;
};