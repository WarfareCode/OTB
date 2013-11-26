namespace
{

/****************************************************************************/
//
// 'SQL_DB_CREATE' Generated from file 'sql_db_create.sql'
const char* SQL_DB_CREATE[] = {
"-- Enable foreign keys support\n"
"PRAGMA foreign_keys = ON\n"
";",
"-----------------------------------------------------------------------------\n"
"CREATE TABLE dataset\n"
"(\n"
"        id    INTEGER PRIMARY KEY AUTOINCREMENT,\n"
"        hash  TEXT NOT NULL,\n"
"        alias TEXT\n"
")\n"
";",
"-----------------------------------------------------------------------------\n"
"CREATE TABLE tag\n"
"(\n"
"        id    INTEGER PRIMARY KEY AUTOINCREMENT,\n"
"        label TEXT NOT NULL DEFAULT 'Label'\n"
")\n"
";",
"CREATE UNIQUE INDEX idx_tag_label ON tag( label )\n"
";",
"-----------------------------------------------------------------------------\n"
"CREATE TABLE tag_node\n"
"(\n"
"        id        INTEGER PRIMARY KEY AUTOINCREMENT,\n"
"        parent_id INTEGER REFERENCES tag_node( id ),\n"
"        tag_id    INTEGER NOT NULL REFERENCES tag( id ),\n"
"        level     INTEGER,\n"
"        path      TEXT\n"
")\n"
";",
"CREATE INDEX idx_tag_node_tid ON tag_node( tag_id )\n"
";",
"-----------------------------------------------------------------------------\n"
"CREATE TABLE dataset_membership(\n"
"        -- id              INTEGER PRIMARY KEY,\n"
"        dataset_id INTEGER NOT NULL REFERENCES dataset( id ),\n"
"        tag_id     INTEGER NOT NULL REFERENCES tag( id ),\n"
"        PRIMARY KEY( dataset_id, tag_id )\n"
")\n"
";",
"CREATE INDEX idx_ds_membership_dataset_id ON dataset_membership( dataset_id )\n"
";",
"CREATE INDEX idx_ds_membership_dataset_tag_id ON dataset_membership( tag_id )\n"
";",
"-----------------------------------------------------------------------------\n"
"CREATE TABLE dataset_attribute\n"
"(\n"
"        id         INTEGER PRIMARY KEY AUTOINCREMENT,\n"
"        dataset_id INTEGER NOT NULL REFERENCES dataset( id ),\n"
"        rank       INTEGER NOT NULL,\n"
"        name       TEXT NOT NULL,\n"
"        value      TEXT\n"
")\n"
";",
"CREATE INDEX idx_dataset_attribute_dsid ON dataset_attribute( dataset_id )\n"
";",
};

const int SQL_DB_CREATE_COUNT = 11;

/****************************************************************************/
//
// 'SQL_DB_SETUP' Generated from file 'sql_db_setup.sql'
const char* SQL_DB_SETUP[] = {
"-----------------------------------------------------------------------------\n"
"INSERT INTO tag( id, label ) VALUES( 1, 'Root' )\n"
";",
"INSERT INTO tag( id, label ) VALUES( 2, 'Datasets' )\n"
";",
"INSERT INTO tag( id, label ) VALUES( 3, 'Cached' )\n"
";",
"-----------------------------------------------------------------------------\n"
"-- Root\n"
"INSERT INTO tag_node( id, parent_id, tag_id, level, path )\n"
"VALUES( 1, NULL, 1, 1, '/')\n"
";",
"-- Root/Datasets\n"
"INSERT INTO tag_node( id, parent_id, tag_id, level, path )\n"
"VALUES( 2, 1,    2, 2, '/1' )\n"
";",
"-- Root/Datasets/Cached\n"
"INSERT INTO tag_node( id, parent_id, tag_id, level, path )\n"
"VALUES( 3, 2,    3, 3, '/1/2' )\n"
";",
};

const int SQL_DB_SETUP_COUNT = 6;

/****************************************************************************/
//
// 'SQL_QUERIES_INSERT' Generated from file 'sql_queries_insert.sql'
const char* SQL_QUERIES_INSERT[] = {
"-----------------------------------------------------------------------------\n"
"-- Insert tag-node for tag :child_label under parent-node of tag-node\n"
"--  :parent_label.\n"
"INSERT INTO tag_node( parent_id, tag_id, level, path )\n"
"        SELECT -- ID (automatic)\n"
"               tag_node.id AS parent_id,\n"
"               (SELECT tag.id FROM tag WHERE tag.label=:child_label) AS tag_id,\n"
"               tag_node.level+1 AS level,\n"
"               rtrim( tag_node.path, '/' ) || '/' || tag_node.id AS path\n"
"        FROM   tag_node\n"
"        JOIN   tag ON tag_node.tag_id=tag.id\n"
"        WHERE  tag.id=(SELECT tag.id FROM tag WHERE tag.label=:parent_label)\n"
";",
};

const int SQL_QUERIES_INSERT_COUNT = 1;

/****************************************************************************/
//
// 'SQL_QUERIES_SELECT' Generated from file 'sql_queries_select.sql'
const char* SQL_QUERIES_SELECT[] = {
"-----------------------------------------------------------------------------\n"
"-- List datasets tagged by given label.\n"
"SELECT dataset.id, tag.label, dataset.alias, dataset.hash\n"
"FROM dataset\n"
"JOIN dataset_membership ON dataset.id=dataset_membership.dataset_id\n"
"JOIN tag ON dataset_membership.tag_id=tag.id\n"
"WHERE tag.label='Datasets'\n"
";",
"-----------------------------------------------------------------------------\n"
"-- List tags marking a given dataset ordered by tree level.\n"
"SELECT tag_node.path, tag_node.level, tag.id AS id, tag.label\n"
"FROM tag_node\n"
"JOIN tag ON tag_node.tag_id=tag.id\n"
"JOIN dataset_membership ON tag.id=dataset_membership.tag_id\n"
"WHERE dataset_membership.dataset_id=2\n"
"ORDER BY tag_node.level\n"
";",
"-----------------------------------------------------------------------------\n"
"-- List tag-node given tag-label.\n"
"SELECT tag.label,\n"
"       tag_node.id,\n"
"       tag_node.parent_id,\n"
"       tag_node.tag_id,\n"
"       tag_node.level,\n"
"       tag_node.path\n"
"FROM tag_node\n"
"JOIN tag ON tag_node.tag_id=tag.id\n"
"WHERE tag_node.tag_id=2\n"
"ORDER BY tag_node.level\n"
";",
};

const int SQL_QUERIES_SELECT_COUNT = 3;
} // End of anonymous namespace.
