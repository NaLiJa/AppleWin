#include "linux/keyboard.h"

#include "StdAfx.h"
#include "AppleWin.h"
#include "YamlHelper.h"

#include <queue>

namespace
{
  std::queue<BYTE> keys;
  BYTE keycode = 0;
}

void addKeyToBuffer(BYTE key)
{
  keys.push(key);
}

BYTE KeybGetKeycode()
{
  return keycode;
}

BYTE KeybReadData()
{
  LogFileTimeUntilFirstKeyRead();

  if (keys.empty())
  {
    return keycode;
  }
  else
  {
    keycode = keys.front();
    const BYTE result = keycode | 0x80;
    return result;
  }
}

BYTE KeybReadFlag()
{
  if (!keys.empty())
  {
    keys.pop();
  }

  return KeybReadData();
}

#define SS_YAML_KEY_LASTKEY "Last Key"
#define SS_YAML_KEY_KEYWAITING "Key Waiting"

static std::string KeybGetSnapshotStructName(void)
{
  static const std::string name("Keyboard");
  return name;
}

void KeybSaveSnapshot(YamlSaveHelper& yamlSaveHelper)
{
  YamlSaveHelper::Label state(yamlSaveHelper, "%s:\n", KeybGetSnapshotStructName().c_str());
  yamlSaveHelper.SaveHexUint8(SS_YAML_KEY_LASTKEY, keycode);
  yamlSaveHelper.SaveBool(SS_YAML_KEY_KEYWAITING, keys.empty() ? false : false);
}

void KeybLoadSnapshot(YamlLoadHelper& yamlLoadHelper, UINT version)
{
  if (!yamlLoadHelper.GetSubMap(KeybGetSnapshotStructName()))
    return;

  keycode = (BYTE) yamlLoadHelper.LoadUint(SS_YAML_KEY_LASTKEY);

  bool keywaiting = false;
  if (version >= 2)
    keywaiting = yamlLoadHelper.LoadBool(SS_YAML_KEY_KEYWAITING);

  keys = std::queue<BYTE>();
  addKeyToBuffer(keycode);

  yamlLoadHelper.PopMap();
}

void KeybReset()
{
}
