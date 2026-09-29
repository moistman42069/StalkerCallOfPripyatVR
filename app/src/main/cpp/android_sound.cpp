#include "xrCore/xrCore.h"
#include "Include/xrAPI/xrAPI.h"
#include "xrSound/Sound.h"

namespace
{
class AndroidSilentSoundScene final : public ISoundScene
{
public:
    void play(ref_sound&, IGameObject*, u32, float) override {}
    void play_at_pos(ref_sound&, IGameObject*, const Fvector&, u32, float) override {}
    void play_no_feedback(ref_sound&, IGameObject*, u32, float, Fvector*, float*, float*, Fvector2*) override {}

    void stop_emitters() const override {}
    int pause_emitters(bool) override { return 0; }

    void set_handler(sound_event*) override {}
    void set_geometry_env(IReader*) override {}
    void set_geometry_som(IReader*) override {}
    void set_geometry_occ(CDB::MODEL*, const Fbox&) override {}

    void set_user_env(CSound_environment*) override {}
    void set_environment(u32, CSound_environment** dst) override
    {
        if (dst)
            *dst = nullptr;
    }
    void set_environment_size(CSound_environment*, CSound_environment** dst) override
    {
        if (dst)
            *dst = nullptr;
    }
    CSound_environment* get_environment(const Fvector&) override { return nullptr; }

    float get_occlusion_to(const Fvector&, const Fvector&, float) override { return 0.f; }
    float get_occlusion(const Fvector&, float, Fvector*) override { return 0.f; }
    void object_relcase(IGameObject*) override {}
};

class AndroidSilentSoundManager final : public ISoundManager
{
    Fvector listenerPosition;

protected:
    CSound* create(pcstr, esound_type, u32) override { return nullptr; }
    void destroy(CSound&) override {}
    void attach_tail(CSound&, pcstr) override {}

public:
    AndroidSilentSoundManager() { listenerPosition.set(0.f, 0.f, 0.f); }

    ISoundScene* create_scene() override { return new AndroidSilentSoundScene(); }
    void destroy_scene(ISoundScene*& scene) override
    {
        delete scene;
        scene = nullptr;
    }

    void _restart() override {}
    bool i_locked() override { return false; }
    void stop_emitters() override {}
    int pause_emitters(bool) override { return 0; }
    void set_master_volume(float) override {}
    void update(const Fvector&, const Fvector&, const Fvector&, const Fvector&) override {}
    void render() override {}
    void statistic(CSound_stats* basic, CSound_stats_ext* extended) override
    {
        if (basic)
            basic->_rendered = basic->_simulated = basic->_events = 0;
        if (extended)
            extended->clear();
    }
    void DumpStatistics(IGameFont&, IPerformanceAlert*) override {}
    const Fvector& listener_position() override { return listenerPosition; }
    void refresh_sources() override {}
};

AndroidSilentSoundManager g_androidSilentSoundManager;
} // namespace

XRSOUND_API u32 snd_device_id = u32(-1);
XRSOUND_API u32 psSoundModel = 0;
XRSOUND_API float psSoundVEffects = 1.f;
XRSOUND_API float psSoundVFactor = 1.f;
XRSOUND_API float psSoundVMusic = 1.f;
XRSOUND_API float psSoundRolloff = 0.75f;
XRSOUND_API float psSoundOcclusionScale = 0.5f;
XRSOUND_API float psSoundTimeFactor = 1.f;
XRSOUND_API Flags32 psSoundFlags = {0};
XRSOUND_API int psSoundTargets = 32;
XRSOUND_API int psSoundCacheSizeMB = 32;
ISoundScene* DefaultSoundScene = nullptr;

void CSoundManager::CreateDevicesList()
{
    soundDevices.clear();
    GEnv.Sound = &g_androidSilentSoundManager;
}

void CSoundManager::Create()
{
    GEnv.Sound = &g_androidSilentSoundManager;
}

void CSoundManager::Destroy()
{
    GEnv.Sound = nullptr;
    soundDevices.clear();
    env_unload();
}

bool CSoundManager::IsSoundEnabled() const { return false; }
void CSoundManager::env_load() {}
void CSoundManager::env_unload() { soundEnvironment = nullptr; }
void CSoundManager::refresh_env_library() {}
SoundEnvironment_LIB* CSoundManager::get_env_library() const { return soundEnvironment; }
