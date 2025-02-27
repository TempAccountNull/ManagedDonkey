#pragma once

#include "cache/cache_file_tag_resource_runtime.hpp"
#include "cseries/cseries.hpp"
#include "memory/secure_signature.hpp"
#include "tag_files/files.hpp"
#include "tag_files/tag_groups.hpp"
#include "tag_files/tag_resource_internals.hpp"

struct s_cache_file_section_file_bounds
{
	long offset;
	long size;
};
static_assert(sizeof(s_cache_file_section_file_bounds) == 0x8);

const long k_cache_file_header_signature = 'head';
const long k_cache_file_footer_signature = 'foot';
const long k_cache_file_version = 18;

#pragma pack(push, 4)
union s_cache_file_header
{
#pragma warning(push)
#pragma warning(disable : 4200)
	byte base[];
#pragma warning(pop)

	struct
	{
		tag header_signature;
		long version;
		long size;

		dword __unknownC;
		dword __unknown10;
		dword __unknown14;
		dword __unknown18;

		long_string source_file;
		string build;
		short scenario_type;
		short scenario_load_type;
		bool __unknown140;
		bool tracked_build;
		bool has_insertion_points;
		byte_flags header_flags;
		s_file_last_modification_date modification_date;

		byte __data14C[0xC];

		long string_id_index_buffer_length;
		long string_id_string_storage_length;
		long string_id_index_buffer;
		long string_id_string_storage;
		dword_flags shared_file_flags; // bool uses_shared_map[shared_file_count];
		s_file_last_modification_date creation_time;
		s_file_last_modification_date shared_file_times[6];
		string name;
		dword game_language;
		long_string relative_path;
		long minor_version;
		long debug_tag_name_count;
		long debug_tag_name_buffer;
		long debug_tag_name_buffer_length;
		long debug_tag_name_offsets;
		s_cache_file_section_file_bounds reports;

		byte __data2E4[0x3C];

		s_network_http_request_hash hash;
		s_rsa_signature rsa_signature;
		c_static_array<long, 4> section_offsets;
		c_static_array<s_cache_file_section_file_bounds, 4> original_section_bounds;
		s_cache_file_shared_resource_usage shared_resource_usage;
		long insertion_point_resource_usage_count; // `has_insertion_points`
		c_static_array<s_cache_file_insertion_point_resource_usage, 9> insertion_point_resource_usage;
		long tag_cache_offsets;
		long tag_count;
		long map_id;
		long scenario_index;
		long cache_file_resource_gestalt_index; // 'zone' tags don't exist in ms23

		byte __data2DF8[0x594];

		tag footer_signature;
	};
};
static_assert(sizeof(s_cache_file_header) == 0x3390);
#pragma pack(pop)

union cache_address
{
	dword value;
	struct
	{
		dword offset : 30;
		dword persistent : 1;
		dword : 1;
	};
};
static_assert(sizeof(cache_address) == 0x4);

union cache_file_tag_instance
{
#pragma warning(push)
#pragma warning(disable : 4200)
	byte base[];
#pragma warning(pop)

	struct
	{
		dword checksum;
		dword total_size;
		short dependency_count;
		short data_fixup_count;
		short resource_fixup_count;
		short : 16;

		// offset from `base`
		dword offset;

		tag group_tags[3];
		c_string_id group_name;

#pragma warning(push)
#pragma warning(disable : 4200)
		dword dependencies[];
#pragma warning(pop)

	};

	bool is_group(tag group_tag)
	{
		return group_tag == group_tags[0] || group_tag == group_tags[1] || group_tag == group_tags[2];
	}

	byte* get()
	{
		return base + offset;
	}
};
static_assert(sizeof(cache_file_tag_instance) == 0x24);

struct s_file_reference_persist
{
	tag signture;
	word_flags flags;
	short location;
	char path[108];
	dword handle;
	long position;
};
static_assert(sizeof(s_file_reference_persist) == 0x7C);

struct s_cache_file_report
{
	char __unknown0[32];
	dword hash[5];
	s_file_reference_persist file_reference;
	dword __unknownB0[20];
	dword __unknown100;
	dword __unknown104;
	dword __unknown108;
	dword __unknown10C;
	dword __unknown110;
};
static_assert(sizeof(s_cache_file_report) == 0x114);

struct s_cache_file_reports
{
	long count;
	s_cache_file_report* elements;
};
static_assert(sizeof(s_cache_file_reports) == 0x8);

union s_cache_file_tags_header
{
#pragma warning(push)
#pragma warning(disable : 4200)
	byte base[];
#pragma warning(pop)

	struct
	{
		dword __unknown0;
		long tag_cache_offsets;
		long tag_count;
		dword __unknownC;
		s_file_last_modification_date creation_date;
		dword __unknown18;
		dword __unknown1C;
	};
};
static_assert(sizeof(s_cache_file_tags_header) == 0x20);

const long k_tag_cache_maximum_files_count = 60000;
const long k_tag_cache_maximum_size = 0x4B00000;

template<long const k_max_file_count>
struct s_cache_file_tag_name_collection
{
	c_static_array<dword, k_max_file_count> offsets;
	c_static_array<char, k_max_file_count * 256> buffer;
	c_static_array<char const*, k_max_file_count> storage;
};
static_assert(sizeof(s_cache_file_tag_name_collection<k_tag_cache_maximum_files_count>) == 0xF1B300);

enum e_cache_file_tag_resource_location_flags
{
	_cache_file_tag_resource_location_flags_valid_checksum = 0,
	_cache_file_tag_resource_location_flags_resources,
	_cache_file_tag_resource_location_flags_textures,
	_cache_file_tag_resource_location_flags_textures_b,
	_cache_file_tag_resource_location_flags_audio,
	_cache_file_tag_resource_location_flags_video,
	_cache_file_tag_resource_location_flags_unused,
	_cache_file_tag_resource_location_flags_only_full_valid_checksum,

	k_cache_file_tag_resource_location_flags_count
};

struct s_cache_file_resource_file_location
{
	// runtime
	short resource_handle_salt;

	c_flags<e_cache_file_tag_resource_location_flags, byte, k_cache_file_tag_resource_location_flags_count> flags;

	char codec;

	union
	{
		// persistent file index, dependant on what location flag is active
		short shared_file_location_index;

		// runtime combined offset, at runtime all resource files are a combined buffer
		dword file_offset;
	};

	dword file_size;
	dword size;
	dword checksum;
	short resource_reference_count;
	short streaming_sublocation_table; // short_block_index
	dword __unknown18;
	dword __unknown1C;
	dword __unknown20;
};
static_assert(sizeof(s_cache_file_resource_file_location) == 0x24);

struct s_cache_file_resource_fixup_location
{
	long encoded_fixup_location;
	long encoded_fixup_value;
};
static_assert(sizeof(s_cache_file_resource_fixup_location) == 0x8);

struct s_tag_resource_interop_location
{
	c_tag_resource_fixup encoded_interop_location;
	long interop_type_index; // long_block_index
};
static_assert(sizeof(s_cache_file_resource_fixup_location) == 0x8);

enum e_cache_file_resource_data_flags
{
	_cache_file_resource_data_flags_has_pageable_data = 0,
	_cache_file_resource_data_flags_has_optional_data,

	k_cache_file_resource_data_flags
};

struct s_cache_file_resource_runtime_data_new
{
	s_tag_reference owner_tag;
	short resource_salt;
	char resource_type_index;
	char control_alignment_bits;
	s_tag_data control_data;
	c_tag_resource_fixup root_fixup;
	c_typed_tag_block<s_cache_file_resource_fixup_location> control_fixups;
	c_typed_tag_block<s_tag_resource_interop_location> interop_locations;
	c_flags<e_cache_file_resource_data_flags, dword, k_cache_file_resource_data_flags> flags;
};
static_assert(sizeof(s_cache_file_resource_runtime_data_new) == 0x48);

struct cache_file_resource_instance
{
	s_cache_file_resource_file_location file_location;
	s_cache_file_resource_runtime_data_new runtime_data;
};
static_assert(sizeof(cache_file_resource_instance) == 0x6C);

struct s_cache_file_tag_resource_data
{
	union
	{
		struct
		{
			long loaded_resource_count;
			cache_file_resource_instance* (&loaded_resources)[k_tag_cache_maximum_files_count];
		} s;

		c_wrapped_array<cache_file_resource_instance*> loaded_resources;
	};

	dword __unknown8;
	dword resource_loaded_size;
	bool __unknown10;

	byte __data14[0x168];
};
static_assert(sizeof(s_cache_file_tag_resource_data) == 0x17C);

struct s_cache_file_globals
{
	s_cache_file_tag_name_collection<k_tag_cache_maximum_files_count>* debug_tag_names;

	// padding?
	dword __unknown4;

	bool tags_loaded;

	// physical_memory_malloc_fixed(sizeof(long) * header.tag_count)
	c_static_array<dword, k_tag_cache_maximum_files_count>& tag_cache_offsets;

	// tag_instances[absolute_index] = tag_cache_base_address[total_tags_size]
	c_static_array<cache_file_tag_instance*, k_tag_cache_maximum_files_count>& tag_instances;

	// tag_index_absolute_mapping[tag_index] = absolute_index;
	c_static_array<dword, k_tag_cache_maximum_files_count>& tag_index_absolute_mapping;

	// absolute_index_tag_mapping[absolute_index] = tag_index;
	c_static_array<dword, k_tag_cache_maximum_files_count>& absolute_index_tag_mapping;

	dword tag_loaded_count;
	dword tag_total_count;

	byte(&tag_cache_base_address)[k_tag_cache_maximum_size];
	dword tag_loaded_size;
	dword tag_cache_size; // k_tag_cache_maximum_size

	s_cache_file_header header;

	// s_cache_file_tags_header* tags_header;
	s_file_reference tags_header;

	s_cache_file_tag_resource_data* resource_data;

	// resource_file_counts_mapping[resource_file_index] = resource_count;
	c_static_array<long, 5> resource_file_counts_mapping;

	dword report_count;
	s_cache_file_report* reports;

	c_static_array<const char*, 5> resource_files;
	const char* map_directory;
};
static_assert(sizeof(s_cache_file_globals) == 0x3508);

extern s_cache_file_globals& g_cache_file_globals;

extern long __cdecl cache_file_get_global_tag_index(tag group_tag);
extern s_cache_file_header const* cache_files_get_header();

extern char const* tag_get_name(long tag_name_index);
extern char const* tag_get_name_safe(long tag_name_index);

struct s_cache_file_security_globals;

extern bool __cdecl cache_file_blocking_read(long cache_file_section, long section_offset, long buffer_size, void* buffer);
extern bool __cdecl cache_file_content_signatures_match(long signature0_size, byte const* signature0, long signature1_size, byte const* signature1, bool unused);
extern bool __cdecl cache_file_get_content_signature(long* out_signature_size, byte const** out_signature);
extern long __cdecl cache_file_get_global_tag_index(tag group_tag);
extern void __cdecl cache_file_get_path(char const* mapname, char* buffer, long buffer_size);
extern s_cache_file_security_globals* __cdecl cache_file_get_security_globals();
extern void const* __cdecl cache_file_globals_get_tag_cache_base_address();
extern bool __cdecl cache_file_header_verify(s_cache_file_header const* header, char const* scenario_path, bool fail_fatally);
extern bool __cdecl cache_file_header_verify_and_version(s_cache_file_header const* header, char const* scenario_path, bool fail_fatally);
extern real __cdecl cache_file_map_progress_estimated_megabytes_remaining(long scenario_type, char const* scenario_path);
extern long __cdecl cache_file_map_progress_estimated_miliseconds_remaining(long scenario_type, char const* scenario_path);
extern real __cdecl cache_file_map_progress_helper(long scenario_type, char const* scenario_path, long progress_type);
extern dword __cdecl cache_files_get_checksum();
extern s_cache_file_header const* __cdecl cache_files_get_header();
extern s_rsa_signature const* __cdecl cache_files_get_rsa_signature();
extern long __cdecl cache_files_get_total_tags_size();
extern char const* __cdecl cache_files_map_directory();
extern bool __cdecl cache_files_verify_header_rsa_signature(s_cache_file_header* header);
extern dword __cdecl compute_realtime_checksum(char* a1, int a2);
extern bool __cdecl scenario_tags_load(char const* scenario_path);
extern void __cdecl scenario_tags_load_finished();
extern void __cdecl scenario_tags_unload();

struct tag_iterator
{
	dword __unknown0;
	dword __unknown4;
	long datum_index;
	long absolute_tag_index;
	tag group_tag;
};
static_assert(sizeof(tag_iterator) == 0x14);

extern void __cdecl tag_files_close();
extern void __cdecl tag_iterator_new(tag_iterator* iterator, tag group_tag);
extern long __cdecl tag_iterator_next(tag_iterator* iterator);

extern bool __cdecl cache_file_tags_load(dword tag_index);
extern void __cdecl cache_file_tags_fixup_all_instances();
extern void* __cdecl tag_get(tag group_tag, long tag_index);
extern void __fastcall sub_503470(s_cache_file_reports* reports, void* unused, cache_file_tag_instance* tag_instance, dword tag_index);
extern void cache_file_tags_load_single_tag_file_test(char const* file_name);

