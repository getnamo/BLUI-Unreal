#include "BluJsonObj.h"
#include "IBlu.h"
#include "Json.h"

UBluJsonObj::UBluJsonObj(const class FObjectInitializer& PCIP)
: Super(PCIP)
{

}

void UBluJsonObj::Init(const FString &StringData)
{
	StrData = *StringData;

	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(StringData);
	DoParseJson(JsonReader);
}

FString UBluJsonObj::GetStringValue(const FString& Index)
{
	return JsonParsed->GetStringField(Index);
}

bool UBluJsonObj::GetBooleanValue(const FString &Index)
{
	return JsonParsed->GetBoolField(Index);
}

float UBluJsonObj::GetNumValue(const FString &Index)
{
	return JsonParsed->GetNumberField(Index);
}

UBluJsonObj* UBluJsonObj::GetNestedObject(const FString &Index)
{
	TSharedPtr<FJsonObject> NewJson = JsonParsed->GetObjectField(Index);

	if (!NewJson.IsValid())
	{
		return nullptr;
	}

	// Make our new Temp obj
	UBluJsonObj* TempObj = NewObject<UBluJsonObj>(GetTransientPackage(), UBluJsonObj::StaticClass());
	TempObj->SetJsonObj(NewJson);

	// return it
	return TempObj;
}

TArray<float> UBluJsonObj::GetNumArray(const FString &Index)
{
	TArray<float> Temp;

	for (TSharedPtr<FJsonValue> Val : JsonParsed->GetArrayField(Index))
	{

		Temp.Add(Val->AsNumber());

	}

	return Temp;
}

TArray<bool> UBluJsonObj::GetBooleanArray(const FString &Index)
{
	TArray<bool> Temp;

	for (TSharedPtr<FJsonValue> Val : JsonParsed->GetArrayField(Index))
	{

		Temp.Add(Val->AsBool());

	}

	return Temp;
}

TArray<FString> UBluJsonObj::GetStringArray(const FString &Index)
{
	TArray<FString> Temp;

	for (TSharedPtr<FJsonValue> Val : JsonParsed->GetArrayField(Index))
	{

		Temp.Add(Val->AsString());

	}

	return Temp;
}


void UBluJsonObj::SetStringValue(const FString &Value, const FString &Index)
{
	JsonParsed->SetStringField(Index, Value);
}

void UBluJsonObj::SetNumValue(const float Value, const FString &Index)
{
	JsonParsed->SetNumberField(Index, Value);
}

void UBluJsonObj::SetBooleanValue(const bool Value, const FString &Index)
{
	JsonParsed->SetBoolField(Index, Value);
}

void UBluJsonObj::SetNestedObject(UBluJsonObj *Value, const FString &Index)
{
	JsonParsed->SetObjectField(Index, Value->GetJsonObj());
}

void UBluJsonObj::SetJsonObj(TSharedPtr<FJsonObject> NewJson)
{
	// Set our new stored JSON object
	JsonParsed = NewJson;
}

TSharedPtr<FJsonObject> UBluJsonObj::GetJsonObj()
{
	return JsonParsed;
}

void UBluJsonObj::DoParseJson(TSharedRef<TJsonReader<TCHAR>> JsonReader)
{
	if (!FJsonSerializer::Deserialize(JsonReader, JsonParsed))
	{
		UE_LOG(LogBlu, Warning, TEXT("JSON STRING FAILED TO PARSE! WILL DEFAULT TO EMPTY OBJECT {}"));

		// Make an empty json object to prevent crashing
		DoParseJson(TJsonReaderFactory<TCHAR>::Create("{}"));
	}
}

// CUSTOM ADDED START
void UBluJsonObj::SetStringArray(const TArray<FString> &Value, const FString &Index)
{
	TArray<TSharedPtr<FJsonValue>> ValueArray;

	for (FString Val : Value)
	{
		ValueArray.Add(MakeShareable(new FJsonValueString(Val)));
	}

	JsonParsed->SetArrayField(Index, ValueArray);
}

void UBluJsonObj::SetBooleanArray(const TArray<bool> &Value, const FString &Index)
{
	TArray<TSharedPtr<FJsonValue>> ValueArray;

	for (bool Val : Value)
	{
		ValueArray.Add(MakeShareable(new FJsonValueBoolean(Val)));
	}

	JsonParsed->SetArrayField(Index, ValueArray);
}

void UBluJsonObj::SetNumArray(const TArray<float> &Value, const FString &Index)
{
	TArray<TSharedPtr<FJsonValue>> ValueArray;

	for (float Val : Value)
	{
		ValueArray.Add(MakeShareable(new FJsonValueNumber(Val)));
	}

	JsonParsed->SetArrayField(Index, ValueArray);
}

void UBluJsonObj::SetObjectArray(const TArray<UBluJsonObj*> &Value, const FString &Index)
{
	TArray<TSharedPtr<FJsonValue>> ValueArray;

	for (UBluJsonObj* Val : Value)
	{
		ValueArray.Add(MakeShareable(new FJsonValueObject(Val->GetJsonObj())));
	}

	JsonParsed->SetArrayField(Index, ValueArray);
}
// CUSTOM ADDED END
