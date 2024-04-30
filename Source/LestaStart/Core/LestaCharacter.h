//	Describes player character 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputAction.h"

#include "Kismet/GameplayStatics.h"
#include "../ActorInterface.h"

#include "../HealthComponent.h"
#include "../GrenadeShootComponent.h"
//#include "../LazerShootUserComponent.h"

#include "../TracePlayersComponent.h"
#include "../LazerShootComponent.h"

#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

#include "LestaCharacter.generated.h"

class UCameraComponent;

UCLASS()
class LESTASTART_API ALestaCharacter : public ACharacter, public IActorInterface
{
	GENERATED_BODY()

private:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FShootingStatus, bool, Flag);
	FShootingStatus ShootingStatus;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartReload);
	FStartReload StartReload;

	enum class WEAPON_TYPE {
		LAZER,
		GRENADE,
	};

	bool IsShooting,IsReloading;

	WEAPON_TYPE ChoisenWeapon;

public:
	ALestaCharacter();

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
		virtual void GetDamage(const double& Damage) override;

	UPROPERTY(EditDefaultsOnly)
		TObjectPtr<UHealthComponent> HealthComp;

	UPROPERTY(EditDefaultsOnly)
		TObjectPtr<UGrenadeShootComponent> GrenadeComp;

	//UPROPERTY(EditDefaultsOnly)
		//TObjectPtr<ULazerShootUserComponent> LazerComp;

	UPROPERTY(EditDefaultsOnly)
		TObjectPtr<UTracePlayersComponent> TraceComp;

	UPROPERTY(EditDefaultsOnly)
		TObjectPtr<ULazerShootComponent> LazerComp;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReloadingDelegate, bool, ReloadingFlag);
	FReloadingDelegate Reloading;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChangeHPDelegate, double, HP);
	FChangeHPDelegate ChangeHP;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChangeWeaponDelegate, FString, Weapon);
	FChangeWeaponDelegate ChangeWeapon;

protected:

	double GrenadeTimeCount;

	UFUNCTION()
		void KillPlayer();

	UFUNCTION()
		void EndOfReload();

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputMappingContext> IMC;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> CameraComponent;

	/** Input action assigned to place defence. */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> PlaceDefenceAction;

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

	virtual void OnPlaceDefenceInput(const FInputActionInstance& InputActionInstance);
	virtual void OnShootInput(const FInputActionInstance& InputActionInstance);
	virtual void OnChangeWeaponInput(const FInputActionInstance& InputActionInstance);
	virtual void OnReloadInput(const FInputActionInstance& InputActionInstance);

	virtual void OnMoveInput(const FInputActionInstance& InputActionInstance);
	virtual void OnLookInput(const FInputActionInstance& InputActionInstance);

	virtual void BeginPlay() override;
};