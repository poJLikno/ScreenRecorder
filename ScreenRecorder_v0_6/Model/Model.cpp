#include "Model.h"

#include "../Controller/Controller.h"

Model::Model()
{
    Controller::model = this;

    model_vars.video_recorder.AttachDevice(&model_vars.video_device);
    model_vars.video_recorder.AttachDevice(&model_vars.video_codec);
    model_vars.video_recorder.AttachDevice(&model_vars.audio_device);
    model_vars.video_recorder.AttachDevice(&model_vars.audio_codec);

    model_vars.video_recorder.AttachPreviewWindow(&model_vars.preview_window);

    //model_vars.video_recorder.Start();
}

ModelVars *Model::QueryModelVars()
{
    return &model_vars;
}

void Model::StartRecording()
{
    model_vars.video_recorder.Stop();
    model_vars.video_recorder.AllowRecording(true);
    model_vars.video_recorder.Start();
}

void Model::StopRecording()
{
    model_vars.video_recorder.Stop();
    model_vars.video_recorder.AllowRecording(false);
    model_vars.video_recorder.Start();
}

void Model::SelectDevice(unsigned int index, Device *device)
{
    if (index < 0) return;

    model_vars.video_recorder.Stop();
    device->SelectDevice(index);
    model_vars.video_recorder.Start();
}

void Model::ShowPreview()
{
    model_vars.video_recorder.Stop();
    model_vars.video_recorder.AllowPreview(true);
    model_vars.video_recorder.Start();
}

void Model::HidePreview()
{
    model_vars.video_recorder.Stop();
    model_vars.video_recorder.AllowPreview(false);
    model_vars.video_recorder.Start();
}

void Model::ShowVideoFilterSettings()
{
    model_vars.video_recorder.Stop();
    model_vars.video_recorder.ShowVideoFilterSettings(true);
    model_vars.video_recorder.Start();
    model_vars.video_recorder.ShowVideoFilterSettings(false);
}

void Model::ShowVideoPinSettings()
{
    model_vars.video_recorder.Stop();
    model_vars.video_recorder.ShowVideoPinSettings(true);
    model_vars.video_recorder.Start();
    model_vars.video_recorder.ShowVideoPinSettings(false);
}

void Model::ShowAudioFilterSettings()
{
    model_vars.video_recorder.Stop();
    model_vars.video_recorder.ShowAudioFilterSettings(true);
    model_vars.video_recorder.Start();
    model_vars.video_recorder.ShowAudioFilterSettings(false);
}

void Model::ShowAudioPinSettings()
{
    model_vars.video_recorder.Stop();
    model_vars.video_recorder.ShowAudioPinSettings(true);
    model_vars.video_recorder.Start();
    model_vars.video_recorder.ShowAudioPinSettings(false);
}
