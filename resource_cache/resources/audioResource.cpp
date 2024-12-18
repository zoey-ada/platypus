#include "audioResource.hpp"

namespace platypus
{

AudioResource::AudioResource(AudioData* resource_data)
	: Resource(resource_data),
	  _channels(resource_data->channels),
	  _sample_rate(resource_data->sample_rate),
	  _bits_per_sample(resource_data->bits_per_sample),
	  _audio_data(resource_data->audio_data),
	  _audio_data_size(resource_data->audio_data_size)
{}

AudioResource::~AudioResource()
{
	if (this->_audio_data != nullptr)
		delete[] this->_audio_data;
	this->_audio_data = nullptr;
}

};
