/*
  Copyright (c) 2016, 2017, Oracle and/or its affiliates. All rights reserved.

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; version 2 of the License.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301  USA */

#ifndef TABLE_REPLICATION_APPLIER_DEFAULT_FILTERS_H
#define TABLE_REPLICATION_APPLIER_DEFAULT_FILTERS_H

/**
  @file storage/perfschema/table_replication_applier_global_filters.h
  Table replication_applier_global_filters (declarations).
*/

#include <sys/types.h>

#include "my_io.h"
#include "mysql_com.h"
#include "pfs_column_types.h"
#include "pfs_engine_table.h"
#include "rpl_info.h" /* CHANNEL_NAME_LENGTH*/
#include "rpl_mi.h"
#include "rpl_msr.h"
#include "table_helper.h"

/** A row in the table */
struct st_row_applier_global_filters
{
  /*
    REPLICATE_DO_DB, REPLICATE_IGNORE_DB, REPLICATE_DO_TABLE,
    REPLICATE_IGNORE_TABLE, REPLICATE_WILD_DO_TABLE,
    REPLICATE_WILD_IGNORE_TABLE, REPLICATE_REWRITE_DB.
  */
  char filter_name[NAME_LEN];
  uint filter_name_length;
  /*
    The replication filter configured by startup options: --replicate-*,
    CHANGE REPLICATION FILTER, or DEFAULT_FILTER (every channel copyies
    global replication filters to its per-channel replication filters
    if there are no per-channel replication filters and there are global
    filters on the filter type when it is created).
  */
  String filter_rule;
  /*
    The global replication filters can be configured with the following
    two states:
    STARTUP_OPTIONS,  //STARTUP_OPTIONS: --REPLICATE-*
    CHANGE_REPLICATION_FILTER //CHANGE REPLICATION FILTER filter [, filter...]
  */
  enum_configured_by configured_by;

  /* Timestamp of when the configuration took place */
  ulonglong active_since;
};

/** Table PERFORMANCE_SCHEMA.replication_applier_global_filters */
class table_replication_applier_global_filters : public PFS_engine_table
{
private:
  /**
    Make a row by an object of Rpl_pfs_filter.

    @param rpl_pfs_filter a pointer to a Rpl_pfs_filter object.
  */
  void make_row(Rpl_pfs_filter *rpl_pfs_filter);

  /** Table share lock. */
  static THR_LOCK m_table_lock;
  /** Table definition. */
  static Plugin_table m_table_def;

  /** Current row */
  st_row_applier_global_filters m_row;
  /** True is the current row exists. */
  bool m_row_exists;
  /** Current position. */
  PFS_simple_index m_pos;
  /** Next position. */
  PFS_simple_index m_next_pos;

protected:
  /**
    Read the current row values.
    @param table            Table handle
    @param buf              row buffer
    @param fields           Table fields
    @param read_all         true if all columns are read.

    @retval 0 if HAVE_REPLICATION is defined, else HA_ERR_RECORD_DELETED.
  */
  virtual int read_row_values(TABLE *table,
                              unsigned char *buf,
                              Field **fields,
                              bool read_all);

  table_replication_applier_global_filters();

public:
  ~table_replication_applier_global_filters();

  /** Table share. */
  static PFS_engine_table_share m_share;
  static PFS_engine_table *create(PFS_engine_table_share *);
  /**
    Get the table count.

    @retval return the table count.
  */
  static ha_rows get_row_count();
  /**
    Fetch the next row in this cursor.

    @retval
      0    Did not reach the end of the table.
      HA_ERR_END_OF_FILE    reached the end of the table.
  */
  virtual int rnd_next();
  /**
    Fetch a row by position.

    @param pos position to fetch
  */
  virtual int rnd_pos(const void *pos);
  virtual void reset_position(void);
};

/** @} */
#endif
