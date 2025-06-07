#pragma once

#include "ACU/basic_types.h"
#include "Serialization/Serialization.h"
#include "Serialization/NumericAdapters.h"
#include "vmath/vmath.h"
struct MyColorPresetData
{
    Vector4f PrimaryColor;
    Vector4f SecondaryColor;
    Vector4f TertiaryColor;
    Vector4f Color4;
    Vector4f Color5;
    Vector4f Color6;
    Vector4f LeatherColor;
    float Velvet = 0;

    MyColorPresetData() {}
    MyColorPresetData(
        const Vector4f& primary
        , const Vector4f& secondary
        , const Vector4f& tertiary
        , const Vector4f& color4
        , const Vector4f& color5
        , const Vector4f& color6
        , const Vector4f& leather
        , float velvet
    )
        : PrimaryColor(primary)
        , SecondaryColor(secondary)
        , TertiaryColor(tertiary)
        , Color4(color4)
        , Color5(color5)
        , Color6(color6)
        , LeatherColor(leather)
        , Velvet(velvet)
    {}

public:
    JSON ToJSON()
    {
        JSON result = json::Object();
        WRITE_JSON_VARIABLE(result, PrimaryColor, NumericVectorAdapter);
        WRITE_JSON_VARIABLE(result, SecondaryColor, NumericVectorAdapter);
        WRITE_JSON_VARIABLE(result, TertiaryColor, NumericVectorAdapter);
        WRITE_JSON_VARIABLE(result, Color4, NumericVectorAdapter);
        WRITE_JSON_VARIABLE(result, Color5, NumericVectorAdapter);
        WRITE_JSON_VARIABLE(result, Color6, NumericVectorAdapter);
        WRITE_JSON_VARIABLE(result, LeatherColor, NumericVectorAdapter);
        WRITE_JSON_VARIABLE(result, Velvet, NumericAdapter);
        return result;
    }
    bool FromJSON(JSON& obj)
    {
        READ_JSON_VARIABLE(obj, PrimaryColor, NumericVectorAdapter);
        READ_JSON_VARIABLE(obj, SecondaryColor, NumericVectorAdapter);
        READ_JSON_VARIABLE(obj, TertiaryColor, NumericVectorAdapter);
        READ_JSON_VARIABLE(obj, Color4, NumericVectorAdapter);
        READ_JSON_VARIABLE(obj, Color5, NumericVectorAdapter);
        READ_JSON_VARIABLE(obj, Color6, NumericVectorAdapter);
        READ_JSON_VARIABLE(obj, LeatherColor, NumericVectorAdapter);
        READ_JSON_VARIABLE(obj, Velvet, NumericAdapter);
        return true;
    }
};
class MyColorPresetDataAdapter : public JSONAdapter<MyColorPresetData, JSON::Class::Object>
{
public:
    using BaseAdapter::BaseAdapter;
    JSON ToJSON();
    bool FromJSON(JSON& obj);
};
JSON MyColorPresetDataAdapter::ToJSON()
{
    return this->source.ToJSON();
}
bool MyColorPresetDataAdapter::FromJSON(JSON& obj)
{
    if (!this->IsCorrectJSONType(obj))
    {
        return false;
    }
    this->source.FromJSON(obj);
    return true;
}

struct ColorsetDescription
{
    const char* m_Name;
    uint64 m_InventoryItemHandle;
    uint8 m_IndexInColorsetsBuildtable;
    MyColorPresetData m_DefaultColors;
};
enum class ColorsetValueType : uint32
{
    Vector4 = 0xD0000,
    SingleFloat = 0xA0000,
};
struct ColorsetValueDescription
{
    const char* Role; // "Primary color", "Secondary Color", "Velvet" etc.
    ColorsetValueType ValueType;
    uint8 IndexCorrespondingToTheBuildColumn;
    bool DoHideBecauseNotUseful; // Colors 4, 5, 6 are just not really useful.
    void* (*MapToCStructMember)(MyColorPresetData& preset);
};
const std::vector<ColorsetDescription> g_ColorsetsDescriptions = {
    ColorsetDescription{ "Assassin Blue", 152805614610, 1,
        MyColorPresetData(
            Vector4f(1.020f, 0.500f, 1.000f, 0.000f),
            Vector4f(0.620f, 1.200f, 1.000f, 0.000f),
            Vector4f(0.000f, 0.000f, 1.100f, 0.000f),
            Vector4f(1.020f, 0.500f, 1.000f, 0.000f),
            Vector4f(0.620f, 1.200f, 1.000f, 0.000f),
            Vector4f(0.000f, 0.000f, 1.100f, 0.000f),
            Vector4f(1.430f, 0.740f, 0.360f, 0.000f),
            0.050f
        )},
    ColorsetDescription{ "Utopia Blue", 152805615012, 8,
        MyColorPresetData(
            Vector4f(1.020f, 0.730f, 1.000f, 1.000f),
            Vector4f(1.020f, 0.730f, 1.000f, 1.000f),
            Vector4f(1.020f, 0.730f, 1.000f, 1.000f),
            Vector4f(1.020f, 0.730f, 1.000f, 1.000f),
            Vector4f(1.020f, 0.730f, 1.000f, 1.000f),
            Vector4f(1.020f, 0.730f, 1.000f, 1.000f),
            Vector4f(1.430f, 1.070f, 0.600f, 1.000f),
            0.080f
        )},
    ColorsetDescription{ "Azure Hope", 152805614952, 18,
        MyColorPresetData(
            Vector4f(1.020f, 0.730f, 1.000f, 1.000f),
            Vector4f(1.530f, 0.650f, 1.000f, 1.000f),
            Vector4f(0.390f, 0.010f, 1.000f, 1.000f),
            Vector4f(1.020f, 0.730f, 1.000f, 1.000f),
            Vector4f(1.530f, 0.650f, 1.000f, 1.000f),
            Vector4f(0.390f, 0.010f, 1.000f, 1.000f),
            Vector4f(0.430f, 1.390f, 0.400f, 1.000f),
            0.080f
        )},
    ColorsetDescription{ "Turquoise Night", 167391274846, 25,
        MyColorPresetData(
            Vector4f(1.070f, 0.400f, 1.090f, 1.000f),
            Vector4f(0.100f, 0.090f, 1.450f, 0.000f),
            Vector4f(0.530f, 0.200f, 1.000f, 0.000f),
            Vector4f(1.070f, 0.400f, 1.090f, 1.000f),
            Vector4f(0.100f, 0.090f, 1.450f, 0.000f),
            Vector4f(0.530f, 0.200f, 1.000f, 0.000f),
            Vector4f(0.510f, 0.450f, 0.680f, 0.000f),
            0.080f
        )},
    ColorsetDescription{ "Tempest", 167391274862, 26,
        MyColorPresetData(
            Vector4f(0.140f, 0.600f, 1.000f, 1.000f),
            Vector4f(0.620f, 1.700f, 1.000f, 0.000f),
            Vector4f(0.470f, 0.240f, 1.000f, 0.000f),
            Vector4f(0.140f, 0.600f, 1.000f, 1.000f),
            Vector4f(0.620f, 1.700f, 1.000f, 0.000f),
            Vector4f(0.470f, 0.240f, 1.000f, 0.000f),
            Vector4f(1.600f, 1.200f, 0.540f, 0.000f),
            0.080f
        )},
    ColorsetDescription{ "Uplay", 152805614940, 15,
        MyColorPresetData(
            Vector4f(0.100f, 0.650f, 1.000f, 0.000f),
            Vector4f(0.520f, 0.800f, 1.500f, 0.000f),
            Vector4f(1.000f, 0.000f, 1.000f, 0.000f),
            Vector4f(0.100f, 0.650f, 1.000f, 0.000f),
            Vector4f(0.520f, 0.800f, 1.500f, 0.000f),
            Vector4f(1.000f, 0.000f, 1.000f, 0.000f),
            Vector4f(0.520f, 1.360f, 0.440f, 0.000f),
            0.080f
        )},
    ColorsetDescription{ "Dark Woods", 152805615008, 2,
        MyColorPresetData(
            Vector4f(1.270f, 0.440f, 1.320f, 0.000f),
            Vector4f(0.560f, 0.940f, 1.180f, 0.000f),
            Vector4f(0.550f, 0.930f, 1.000f, 0.000f),
            Vector4f(1.270f, 0.440f, 1.320f, 0.000f),
            Vector4f(0.560f, 0.940f, 1.180f, 0.000f),
            Vector4f(0.550f, 0.930f, 1.000f, 0.000f),
            Vector4f(1.780f, 0.880f, 0.530f, 0.000f),
            0.060f
        )},
    ColorsetDescription{ "Emerald Regrets", 167391274854, 24,
        MyColorPresetData(
            Vector4f(1.340f, 0.240f, 1.090f, 1.000f),
            Vector4f(0.080f, 0.700f, 1.000f, 0.000f),
            Vector4f(0.520f, 0.450f, 1.000f, 0.000f),
            Vector4f(1.340f, 0.240f, 1.090f, 1.000f),
            Vector4f(0.080f, 0.700f, 1.000f, 0.000f),
            Vector4f(0.520f, 0.450f, 1.000f, 0.000f),
            Vector4f(0.800f, 0.650f, 0.600f, 1.000f),
            0.080f
        )},
    ColorsetDescription{ "Marsh Dweller", 152805614976, 12,
        MyColorPresetData(
            Vector4f(0.350f, 0.450f, 1.000f, 0.000f),
            Vector4f(0.620f, 1.500f, 1.000f, 0.000f),
            Vector4f(1.030f, 0.400f, 1.000f, 0.000f),
            Vector4f(0.350f, 0.450f, 1.000f, 0.000f),
            Vector4f(0.620f, 1.500f, 1.000f, 0.000f),
            Vector4f(1.030f, 0.400f, 1.000f, 0.000f),
            Vector4f(0.770f, 0.900f, 0.620f, 0.000f),
            0.080f
        )},
    ColorsetDescription{ "Deep Forest", 152805614932, 14,
        MyColorPresetData(
            Vector4f(0.280f, 0.720f, 1.650f, 1.000f),
            Vector4f(0.360f, 0.720f, 1.150f, 1.000f),
            Vector4f(0.280f, 0.540f, 1.000f, 1.000f),
            Vector4f(0.280f, 0.720f, 1.650f, 1.000f),
            Vector4f(0.360f, 0.720f, 1.150f, 1.000f),
            Vector4f(0.280f, 0.540f, 1.000f, 1.000f),
            Vector4f(0.770f, 1.300f, 0.800f, 1.000f),
            0.080f
        )},
    ColorsetDescription{ "Infinite Green", 152805614956, 19,
        MyColorPresetData(
            Vector4f(0.510f, 0.680f, 1.000f, 1.000f),
            Vector4f(1.400f, 0.830f, 1.070f, 1.000f),
            Vector4f(0.450f, 0.690f, 1.000f, 1.000f),
            Vector4f(0.510f, 0.680f, 1.000f, 1.000f),
            Vector4f(1.400f, 0.830f, 1.070f, 1.000f),
            Vector4f(0.450f, 0.690f, 1.000f, 1.000f),
            Vector4f(0.920f, 2.200f, 0.440f, 1.000f),
            0.050f
        )},
    ColorsetDescription{ "Shiny Firefly", 152805614996, 5,
        MyColorPresetData(
            Vector4f(0.520f, 1.160f, 4.300f, 1.000f),
            Vector4f(0.520f, 1.160f, 4.300f, 1.000f),
            Vector4f(0.520f, 1.160f, 2.300f, 1.000f),
            Vector4f(0.520f, 1.160f, 4.300f, 1.000f),
            Vector4f(0.520f, 1.160f, 4.300f, 1.000f),
            Vector4f(0.520f, 1.160f, 2.300f, 1.000f),
            Vector4f(0.920f, 2.170f, 0.960f, 0.000f),
            0.080f
        )},
    ColorsetDescription{ "Darkest Alley", 152805614960, 20,
        MyColorPresetData(
            Vector4f(0.500f, 0.100f, 1.000f, 0.000f),
            Vector4f(0.280f, 0.200f, 1.000f, 0.000f),
            Vector4f(0.500f, 0.100f, 1.000f, 0.000f),
            Vector4f(0.500f, 0.100f, 1.000f, 0.000f),
            Vector4f(0.280f, 0.200f, 1.000f, 0.000f),
            Vector4f(0.500f, 0.100f, 1.000f, 0.000f),
            Vector4f(0.860f, 0.500f, 0.150f, 0.000f),
            0.035f
        )},
    ColorsetDescription{ "Pure Shadow", 152805614984, 10,
        MyColorPresetData(
            Vector4f(1.000f, 0.120f, 1.000f, 0.000f),
            Vector4f(0.970f, 0.120f, 1.000f, 0.000f),
            Vector4f(0.970f, 0.120f, 1.000f, 0.000f),
            Vector4f(1.000f, 0.120f, 1.000f, 0.000f),
            Vector4f(0.970f, 0.120f, 1.000f, 0.000f),
            Vector4f(0.970f, 0.120f, 1.000f, 0.000f),
            Vector4f(0.550f, 0.100f, 0.150f, 0.000f),
            0.020f
        )},
    ColorsetDescription{ "Peaceful Tales", 152805614936, 7,
        MyColorPresetData(
            Vector4f(0.000f, 0.000f, 8.000f, 0.000f),
            Vector4f(0.610f, 1.700f, 1.000f, 0.000f),
            Vector4f(1.500f, 0.100f, 1.000f, 1.000f),
            Vector4f(0.000f, 0.000f, 8.000f, 0.000f),
            Vector4f(0.610f, 1.700f, 1.000f, 0.000f),
            Vector4f(1.500f, 0.100f, 1.000f, 1.000f),
            Vector4f(0.000f, 0.000f, 5.000f, 0.000f),
            0.080f
        )},
    ColorsetDescription{ "White Eagle", 152805614980, 11,
        MyColorPresetData(
            Vector4f(0.000f, 0.000f, 8.000f, 0.000f),
            Vector4f(0.000f, 0.000f, 1.000f, 0.000f),
            Vector4f(0.000f, 0.000f, 8.330f, 0.000f),
            Vector4f(0.000f, 0.000f, 8.000f, 0.000f),
            Vector4f(0.000f, 0.000f, 1.000f, 0.000f),
            Vector4f(0.000f, 0.000f, 8.330f, 0.000f),
            Vector4f(0.000f, 0.000f, 5.000f, 0.000f),
            0.080f
        )},
    ColorsetDescription{ "Rainy Sky", 152805615004, 3,
        MyColorPresetData(
            Vector4f(1.000f, 0.000f, 2.600f, 1.000f),
            Vector4f(0.040f, 0.700f, 1.000f, 1.000f),
            Vector4f(1.540f, 0.600f, 1.000f, 1.000f),
            Vector4f(1.000f, 0.000f, 2.600f, 1.000f),
            Vector4f(0.040f, 0.700f, 1.000f, 1.000f),
            Vector4f(1.540f, 0.600f, 1.000f, 1.000f),
            Vector4f(1.000f, 0.000f, 1.500f, 1.000f),
            0.060f
        )},
    ColorsetDescription{ "Brown Sands", 152805614944, 16,
        MyColorPresetData(
            Vector4f(0.530f, 0.400f, 4.000f, 1.000f),
            Vector4f(0.550f, 0.500f, 1.000f, 1.000f),
            Vector4f(0.500f, 0.450f, 1.000f, 1.000f),
            Vector4f(0.530f, 0.400f, 4.000f, 1.000f),
            Vector4f(0.550f, 0.500f, 1.000f, 1.000f),
            Vector4f(0.500f, 0.450f, 1.000f, 1.000f),
            Vector4f(0.960f, 0.700f, 1.770f, 1.000f),
            0.060f
        )},
    ColorsetDescription{ "Beige Ghost", 167391274858, 23,
        MyColorPresetData(
            Vector4f(0.530f, 0.280f, 3.000f, 1.000f),
            Vector4f(0.610f, 1.900f, 1.000f, 0.000f),
            Vector4f(0.530f, 0.280f, 1.000f, 1.000f),
            Vector4f(0.530f, 0.280f, 3.000f, 1.000f),
            Vector4f(0.610f, 1.900f, 1.000f, 0.000f),
            Vector4f(0.530f, 0.280f, 1.000f, 1.000f),
            Vector4f(-0.040f, 0.640f, 1.020f, 1.000f),
            0.080f
        )},
    ColorsetDescription{ "Amber Dawn", 152805614988, 9,
        MyColorPresetData(
            Vector4f(0.560f, 0.760f, 1.000f, 0.000f),
            Vector4f(1.080f, 0.800f, 1.000f, 0.000f),
            Vector4f(0.200f, 0.000f, 1.000f, 0.000f),
            Vector4f(0.560f, 0.760f, 1.000f, 0.000f),
            Vector4f(1.080f, 0.800f, 1.000f, 0.000f),
            Vector4f(0.200f, 0.000f, 1.000f, 0.000f),
            Vector4f(0.990f, 1.220f, 1.000f, 0.000f),
            0.080f
        )},
    ColorsetDescription{ "Autumn Leaves", 152805614972, 13,
        MyColorPresetData(
            Vector4f(0.600f, 1.100f, 1.100f, 1.000f),
            Vector4f(0.470f, 0.590f, 2.300f, 1.000f),
            Vector4f(0.530f, 0.560f, 1.000f, 1.000f),
            Vector4f(0.600f, 1.100f, 1.100f, 1.000f),
            Vector4f(0.470f, 0.590f, 2.300f, 1.000f),
            Vector4f(0.530f, 0.560f, 1.000f, 1.000f),
            Vector4f(0.980f, 2.840f, 0.460f, 1.000f),
            0.060f
        )},
    ColorsetDescription{ "Fancy Man", 152805614964, 21,
        MyColorPresetData(
            Vector4f(0.600f, 1.200f, 2.000f, 1.000f),
            Vector4f(0.600f, 1.200f, 2.000f, 1.000f),
            Vector4f(0.600f, 1.200f, 1.000f, 1.000f),
            Vector4f(0.600f, 1.200f, 2.000f, 1.000f),
            Vector4f(0.600f, 1.200f, 2.000f, 1.000f),
            Vector4f(0.600f, 1.200f, 2.000f, 1.000f),
            Vector4f(0.000f, 2.500f, 0.560f, 1.000f),
            0.080f
        )},
    ColorsetDescription{ "Wildfire", 152805614948, 17,
        MyColorPresetData(
            Vector4f(0.610f, 3.210f, 0.650f, 0.000f),
            Vector4f(0.610f, 2.500f, 0.400f, 0.000f),
            Vector4f(0.610f, 4.100f, 0.170f, 0.000f),
            Vector4f(0.610f, 3.210f, 0.650f, 0.000f),
            Vector4f(0.610f, 2.500f, 0.400f, 0.000f),
            Vector4f(0.610f, 4.100f, 0.170f, 0.000f),
            Vector4f(1.030f, 3.700f, 0.300f, 1.000f),
            0.080f
        )},
    ColorsetDescription{ "Bright Plumage", 167391274850, 27,
        MyColorPresetData(
            Vector4f(0.620f, 1.000f, 1.000f, 1.000f),
            Vector4f(0.550f, 1.000f, 2.000f, 1.000f),
            Vector4f(0.520f, 1.000f, 1.750f, 1.000f),
            Vector4f(0.620f, 1.000f, 1.000f, 1.000f),
            Vector4f(0.550f, 1.000f, 2.000f, 1.000f),
            Vector4f(0.520f, 1.000f, 1.750f, 1.000f),
            Vector4f(0.000f, 3.660f, 0.200f, 0.000f),
            0.080f
        )},
    ColorsetDescription{ "Sunset Valley", 152805615000, 4,
        MyColorPresetData(
            Vector4f(0.610f, 2.000f, 1.000f, 0.000f),
            Vector4f(0.430f, 0.550f, 1.000f, 0.000f),
            Vector4f(0.450f, 0.700f, 1.000f, 0.000f),
            Vector4f(0.610f, 2.000f, 0.810f, 0.000f),
            Vector4f(0.430f, 0.550f, 1.000f, 0.000f),
            Vector4f(0.450f, 0.700f, 1.000f, 0.000f),
            Vector4f(1.030f, 3.660f, 0.360f, 0.000f),
            0.050f
        )},
    ColorsetDescription{ "Winter Roses", 152805614992, 6,
        MyColorPresetData(
            Vector4f(0.740f, 0.700f, 1.000f, 1.000f),
            Vector4f(1.190f, 0.680f, 1.000f, 1.000f),
            Vector4f(0.510f, 0.380f, 1.000f, 1.000f),
            Vector4f(0.740f, 0.700f, 1.000f, 1.000f),
            Vector4f(1.190f, 0.680f, 1.000f, 1.000f),
            Vector4f(0.510f, 0.380f, 1.000f, 1.000f),
            Vector4f(1.120f, 1.880f, 0.260f, 1.000f),
            0.060f
        )},
    ColorsetDescription{ "Royal Purple", 152805614968, 22,
        MyColorPresetData(
            Vector4f(0.840f, 0.400f, 1.000f, 1.000f),
            Vector4f(0.520f, 0.700f, 1.520f, 1.000f),
            Vector4f(0.510f, 0.380f, 1.000f, 1.000f),
            Vector4f(0.840f, 0.400f, 1.000f, 1.000f),
            Vector4f(0.520f, 0.700f, 1.520f, 1.000f),
            Vector4f(0.510f, 0.380f, 1.000f, 1.000f),
            Vector4f(1.200f, 1.300f, 0.200f, 0.000f),
            0.080f
        )},
};
std::vector<ColorsetValueDescription> g_ColorsetValueDescriptions = {
    ColorsetValueDescription{ "Primary color",     ColorsetValueType::Vector4,       1,  false, [](MyColorPresetData& preset) { return (void*)&preset.PrimaryColor; } },
    ColorsetValueDescription{ "Secondary color",   ColorsetValueType::Vector4,       2,  false, [](MyColorPresetData& preset) { return (void*)&preset.SecondaryColor; } },
    ColorsetValueDescription{ "Tertiary color",    ColorsetValueType::Vector4,       3,  false, [](MyColorPresetData& preset) { return (void*)&preset.TertiaryColor; } },
    ColorsetValueDescription{ "Leather color",     ColorsetValueType::Vector4,       10, false, [](MyColorPresetData& preset) { return (void*)&preset.LeatherColor; } },
    ColorsetValueDescription{ "Velvet",            ColorsetValueType::SingleFloat,   9,  false, [](MyColorPresetData& preset) { return (void*)&preset.Velvet; } },
    ColorsetValueDescription{ "Color4",            ColorsetValueType::Vector4,       4,  true,  [](MyColorPresetData& preset) { return (void*)&preset.Color4; } },
    ColorsetValueDescription{ "Color5",            ColorsetValueType::Vector4,       6,  true,  [](MyColorPresetData& preset) { return (void*)&preset.Color5; } },
    ColorsetValueDescription{ "Color6",            ColorsetValueType::Vector4,       7,  true,  [](MyColorPresetData& preset) { return (void*)&preset.Color6; } },
};
