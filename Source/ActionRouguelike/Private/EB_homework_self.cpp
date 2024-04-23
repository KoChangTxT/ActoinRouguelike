// Fill out your copyright notice in the Description page of Project Settings.


#include "EB_homework_self.h"
#include "DrawDebugHelpers.h"

// Sets default values
AEB_homework::AEB_homework()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	RadialForce = CreateDefaultSubobject<URadialForceComponent>("RadialForce");

	RootComponent = StaticMesh;
	RadialForce->SetupAttachment(StaticMesh);

	RadialForce->Radius = 678.0f;
	RadialForce->ImpulseStrength = 2000.0f;
	RadialForce->bImpulseVelChange = true;
	StaticMesh->SetSimulatePhysics(true);

}


void AEB_homework::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	RadialForce->FireImpulse();

	UE_LOG(LogTemp, Log, TEXT("OnHit function")); //三个日志打印函数

	UE_LOG(LogTemp, Warning, TEXT("OtherActor: %s,at game time: %f"),*GetNameSafe(OtherActor),GetWorld()->TimeSeconds);

	FString CombinedString = FString::Printf(TEXT("Hit at location: %s"), *Hit.Location.ToString());
	DrawDebugString(GetWorld(), Hit.ImpactPoint, CombinedString, nullptr, FColor::Green, 2.0f, true);
}

// Called when the game starts or when spawned
void AEB_homework::BeginPlay()
{
	Super::BeginPlay();
	
	StaticMesh->OnComponentHit.AddDynamic(this, &AEB_homework::OnHit);
}

// Called every frame
void AEB_homework::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

