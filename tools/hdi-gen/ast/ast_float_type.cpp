/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
 *
 * HDF is dual licensed: you can use it either under the terms of
 * the GPL, or the BSD license, at your option.
 * See the LICENSE file in the root of this repository for complete details.
 */

#include "ast/ast_float_type.h"

namespace OHOS {
namespace HDI {
bool ASTFloatType::IsFloatType()
{
    return true;
}

String ASTFloatType::ToString()
{
    return "float";
}

TypeKind ASTFloatType::GetTypeKind()
{
    return TypeKind::TYPE_FLOAT;
}

String ASTFloatType::EmitCType(TypeMode mode) const
{
    switch (mode) {
        case TypeMode::NO_MODE:
            return "float";
        case TypeMode::PARAM_IN:
            return "float";
        case TypeMode::PARAM_OUT:
            return "float*";
        case TypeMode::LOCAL_VAR:
            return "float";
        default:
            return "unknow type";
    }
}

String ASTFloatType::EmitCppType(TypeMode mode) const
{
    switch (mode) {
        case TypeMode::NO_MODE:
            return "float";
        case TypeMode::PARAM_IN:
            return "float";
        case TypeMode::PARAM_OUT:
            return "float&";
        case TypeMode::LOCAL_VAR:
            return "float";
        default:
            return "unknow type";
    }
}

String ASTFloatType::EmitJavaType(TypeMode mode, bool isInnerType) const
{
    return isInnerType ? "Float" : "float";
}

void ASTFloatType::EmitCWriteVar(const String& parcelName, const String& name, const String& gotoLabel,
    StringBuilder& sb, const String& prefix) const
{
    sb.Append(prefix).AppendFormat("if (!HdfSbufWriteFloat(%s, %s)) {\n",
        parcelName.string(), name.string());
    sb.Append(prefix + g_tab).AppendFormat(
        "HDF_LOGE(\"%%{public}s: write %s failed!\", __func__);\n", name.string());
    sb.Append(prefix + g_tab).Append("ec = HDF_ERR_INVALID_PARAM;\n");
    sb.Append(prefix + g_tab).AppendFormat("goto %s;\n", gotoLabel.string());
    sb.Append(prefix).Append("}\n");
}

void ASTFloatType::EmitCProxyReadVar(const String& parcelName, const String& name, bool isInnerType,
    const String& gotoLabel, StringBuilder& sb, const String& prefix) const
{
    sb.Append(prefix).AppendFormat("if (!HdfSbufReadFloat(%s, %s)) {\n", parcelName.string(), name.string());
    sb.Append(prefix + g_tab).AppendFormat(
        "HDF_LOGE(\"%%{public}s: read %s failed!\", __func__);\n", name.string());
    sb.Append(prefix + g_tab).Append("ec = HDF_ERR_INVALID_PARAM;\n");
    sb.Append(prefix + g_tab).AppendFormat("goto %s;\n", gotoLabel.string());
    sb.Append(prefix).Append("}\n");
}

void ASTFloatType::EmitCStubReadVar(const String& parcelName, const String& name, StringBuilder& sb,
    const String& prefix) const
{
    sb.Append(prefix).AppendFormat("if (!HdfSbufReadFloat(%s, %s)) {\n", parcelName.string(), name.string());
    sb.Append(prefix + g_tab).AppendFormat(
        "HDF_LOGE(\"%%{public}s: read %s failed!\", __func__);\n", name.string());
    sb.Append(prefix + g_tab).Append("ec = HDF_ERR_INVALID_PARAM;\n");
    sb.Append(prefix + g_tab).Append("goto errors;\n");
    sb.Append(prefix).Append("}\n");
}

void ASTFloatType::EmitCppWriteVar(const String& parcelName, const String& name, StringBuilder& sb,
    const String& prefix, unsigned int innerLevel) const
{
    sb.Append(prefix).AppendFormat("if (!%s.WriteFloat(%s)) {\n", parcelName.string(), name.string());
    sb.Append(prefix + g_tab).AppendFormat(
        "HDF_LOGE(\"%%{public}s: write %s failed!\", __func__);\n", name.string());
    sb.Append(prefix + g_tab).Append("return HDF_ERR_INVALID_PARAM;\n");
    sb.Append(prefix).Append("}\n");
}

void ASTFloatType::EmitCppReadVar(const String& parcelName, const String& name, StringBuilder& sb,
    const String& prefix, bool initVariable, unsigned int innerLevel) const
{
    if (initVariable) {
        sb.Append(prefix).AppendFormat("%s %s = %s.ReadFloat();\n", EmitCppType().string(),
            name.string(), parcelName.string());
    } else {
        sb.Append(prefix).AppendFormat("%s = %s.ReadFloat();\n", name.string(), parcelName.string());
    }
}

void ASTFloatType::EmitCMarshalling(const String& name, StringBuilder& sb, const String& prefix) const
{
    sb.Append(prefix).AppendFormat("if (!HdfSbufWriteFloat(data, %s)) {\n", name.string());
    sb.Append(prefix + g_tab).AppendFormat(
        "HDF_LOGE(\"%%{public}s: write %s failed!\", __func__);\n", name.string());
    sb.Append(prefix + g_tab).Append("return false;\n");
    sb.Append(prefix).Append("}\n");
}

void ASTFloatType::EmitCUnMarshalling(const String& name, StringBuilder& sb, const String& prefix,
    std::vector<String>& freeObjStatements) const
{
    sb.Append(prefix).AppendFormat("if (!HdfSbufReadFloat(data, &%s)) {\n", name.string());
    sb.Append(prefix + g_tab).AppendFormat(
        "HDF_LOGE(\"%%{public}s: read %s failed!\", __func__);\n", name.string());
    EmitFreeStatements(freeObjStatements, sb, prefix + g_tab);
    sb.Append(prefix + g_tab).Append("goto errors;\n");
    sb.Append(prefix).Append("}\n");
}

void ASTFloatType::EmitCppMarshalling(const String& parcelName, const String& name, StringBuilder& sb,
    const String& prefix, unsigned int innerLevel) const
{
    sb.Append(prefix).AppendFormat("if (!%s.WriteFloat(%s)) {\n", parcelName.string(), name.string());
    sb.Append(prefix + g_tab).AppendFormat(
        "HDF_LOGE(\"%%{public}s: write %s failed!\", __func__);\n", name.string());
    sb.Append(prefix + g_tab).Append("return false;\n");
    sb.Append(prefix).Append("}\n");
}

void ASTFloatType::EmitCppUnMarshalling(const String& parcelName, const String& name, StringBuilder& sb,
    const String& prefix, bool emitType, unsigned int innerLevel) const
{
    if (emitType) {
        sb.Append(prefix).AppendFormat("%s %s = %s.ReadFloat();\n",
            EmitCppType().string(), name.string(), parcelName.string());
    } else {
        sb.Append(prefix).AppendFormat("%s = %s.ReadFloat();\n", name.string(), parcelName.string());
    }
}
} // namespace HDI
} // namespace OHOS