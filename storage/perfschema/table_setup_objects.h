/* Copyright (c) 2008, 2017, Oracle and/or its affiliates. All rights reserved.

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; version 2 of the License.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software Foundation,
  51 Franklin Street, Suite 500, Boston, MA 02110-1335 USA */

#ifndef TABLE_SETUP_OBJECTS_H
#define TABLE_SETUP_OBJECTS_H

/**
  @file storage/perfschema/table_setup_objects.h
  Table SETUP_OBJECTS (declarations).
*/

#include <sys/types.h>

#include "pfs_engine_table.h"
#include "table_helper.h"

struct PFS_setup_object;

/**
  @addtogroup performance_schema_tables
  @{
*/

/** A row of PERFORMANCE_SCHEMA.SETUP_OBJECTS. */
struct row_setup_objects
{
  /** Column OBJECT_TYPE. */
  enum_object_type m_object_type;
  /** Column SCHEMA_NAME. */
  char m_schema_name[NAME_LEN];
  /** Length in bytes of @c m_schema_name. */
  uint m_schema_name_length;
  /** Column OBJECT_NAME. */
  char m_object_name[NAME_LEN];
  /** Length in bytes of @c m_object_name. */
  uint m_object_name_length;
  /** Column ENABLED. */
  bool *m_enabled_ptr;
  /** Column TIMED. */
  bool *m_timed_ptr;
};

class PFS_index_setup_objects : public PFS_engine_index
{
public:
  PFS_index_setup_objects()
    : PFS_engine_index(&m_key_1, &m_key_2, &m_key_3),
      m_key_1("OBJECT_TYPE"),
      m_key_2("OBJECT_SCHEMA"),
      m_key_3("OBJECT_NAME")
  {
  }

  ~PFS_index_setup_objects()
  {
  }

  virtual bool match(PFS_setup_object *pfs);
  virtual bool match(row_setup_objects *row);

private:
  PFS_key_object_type_enum m_key_1;
  PFS_key_object_schema m_key_2;
  PFS_key_object_name m_key_3;
};

/** Table PERFORMANCE_SCHEMA.SETUP_OBJECTS. */
class table_setup_objects : public PFS_engine_table
{
public:
  /** Table share. */
  static PFS_engine_table_share m_share;
  /** Table builder. */
  static PFS_engine_table *create(PFS_engine_table_share *);
  static int write_row(PFS_engine_table *pfs_table,
                       TABLE *table,
                       unsigned char *buf,
                       Field **fields);
  static int delete_all_rows();
  static ha_rows get_row_count();

  virtual void reset_position(void);

  virtual int rnd_next();
  virtual int rnd_pos(const void *pos);

  virtual int index_init(uint idx, bool sorted);
  virtual int index_next();

protected:
  virtual int read_row_values(TABLE *table,
                              unsigned char *buf,
                              Field **fields,
                              bool read_all);

  virtual int update_row_values(TABLE *table,
                                const unsigned char *old_buf,
                                unsigned char *new_buf,
                                Field **fields);

  virtual int delete_row_values(TABLE *table,
                                const unsigned char *buf,
                                Field **fields);

  table_setup_objects();

public:
  ~table_setup_objects()
  {
  }

private:
  int make_row(PFS_setup_object *pfs);

  /** Table share lock. */
  static THR_LOCK m_table_lock;
  /** Table definition. */
  static Plugin_table m_table_def;

  /** Current row. */
  row_setup_objects m_row;
  /** Current position. */
  PFS_simple_index m_pos;
  /** Next position. */
  PFS_simple_index m_next_pos;

  PFS_index_setup_objects *m_opened_index;
};

/** @} */
#endif
