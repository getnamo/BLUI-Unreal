#pragma once
#include "../Private/BluPrivatePCH.h"
#include "BluWidget.generated.h"

class BrowserClient;
class RenderHandler;

/*
* Struct for Texture data
* Based on code from VaQuoleUI by Vladimir Alyamkin
*/
struct TextureData
{
	TextureData(uint32 InWidth = 0, uint32 InHeight = 0, uint32 InStride = 0, const TArray<uint32>& InBytes = TArray<uint32>())
	: Bytes(InBytes)
	, Width(InWidth)
	, Height(InHeight)
	, StrideBytes(InStride)
	{

	}

	TextureData(const TextureData &Other)
		: Bytes(Other.Bytes)
		, Width(Other.Width)
		, Height(Other.Height)
		, StrideBytes(Other.StrideBytes)
	{

	}

	TextureData& operator=(const TextureData& Other)
	{
		if (this != &Other)
		{
			SetRawData(Other.Width, Other.Height, Other.StrideBytes, Other.Bytes);
		}
		return *this;
	}

	void SetRawData(uint32 InWidth, uint32 InHeight, uint32 InStride, const TArray<uint32>& InBytes)
	{
		Width = InWidth;
		Height = InHeight;
		StrideBytes = InStride;
		Bytes = InBytes;
	}

	void Empty()
	{
		Bytes.Empty();
	}

	uint32 GetWidth() const { return Width; }
	uint32 GetHeight() const { return Height; }
	uint32 GetStride() const { return StrideBytes; }
	uint32 GetDataSize() const { return Width * Height * StrideBytes; }
	const TArray<uint32>& GetRawBytes() const { return Bytes; }

	/** Accesses the raw bytes of already sized texture data */
	uint32* GetRawBytesPtr() { return Bytes.GetData(); }

private:
	/** Raw uncompressed texture data */
	TArray<uint32> Bytes;

	/** Width of the texture */
	uint32 Width;

	/** Height of the texture */
	uint32 Height;

	/** The number of bytes of each pixel */
	uint32 StrideBytes;

};

typedef TSharedPtr<TextureData, ESPMode::ThreadSafe> TextureDataPtr;
typedef TSharedRef<TextureData, ESPMode::ThreadSafe> TextureDataRef;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FScriptEvent, const FString&, EventName, const FString&, EventMessage);

UCLASS()
class BLU_API UBluWidget : public UUserWidget
{

	GENERATED_UCLASS_BODY()

	/* Initialize function, should be called after properties are set */
	UFUNCTION(BlueprintCallable, Category = "Blu")
		void init();

	/* The default URL this UI component will load */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blu")
		FString DefaultURL;

	/* Is this UI component current active? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blu")
		bool bEnabled;

	/* Should this be rendered in game to be transparent? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blu")
		bool bIsTransparent;

	/* Width of the view resolution */
	UPROPERTY(EditAnywhere, Category = "Blu")
		int32 Width;

	/* Height of the view resolution */
	UPROPERTY(EditAnywhere, Category = "Blu")
		int32 Height;

	/* Material that will be instanced to load UI texture into it */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Blu")
		UMaterialInterface* BaseMaterial;

	/* Name of parameter to load UI texture into material */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Blu")
		FName TextureParameterName;

	/* Get the material instance, apply this to the display mesh */
	//UFUNCTION(BlueprintCallable, Category = "Blu")
	//	UMaterialInstanceDynamic* GetMaterialInstance() const;

	/* Get the texture data from our UI component */
	UFUNCTION(BlueprintCallable, Category = "Blu")
		UTexture2D* GetTexture() const;

	/* Execute JS code inside the browser */
	UFUNCTION(BlueprintCallable, Category = "Blu")
		void ExecuteJS(FString code);

	/* Javascript event emitter */
	UPROPERTY(BlueprintAssignable)
		FScriptEvent ScriptEventEmitter;

	CefRefPtr<CefBrowser> browser;

	void TextureUpdate();

	protected:
		CefWindowInfo info;
		CefRefPtr<BrowserClient> g_handler;
		CefBrowserSettings browserSettings;
		RenderHandler* renderer;

		void ResetTexture();
		void DestroyTexture();
		// void ResetMaterialInstance();

		// Store UI state in this UTexture2D
		UTexture2D* Texture;

		// Material instance 
		UMaterialInstanceDynamic* MaterialInstance;

};