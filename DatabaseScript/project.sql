DROP TABLE
IF EXISTS board_rect;

DROP TABLE
IF EXISTS source_board;

DROP TABLE
IF EXISTS cutting_solution;

DROP TABLE
IF EXISTS material_group;

DROP TABLE
IF EXISTS product_board;

DROP TABLE
IF EXISTS material_list;

CREATE TABLE material_list (
	mlid CHAR (36) PRIMARY KEY,
	name TEXT NOT NULL
);

CREATE TABLE product_board (
	pbid CHAR (36) PRIMARY KEY,
	mlid CHAR (36) NOT NULL,
	orders_no TEXT,
	cabine_no TEXT,
	barcode_no TEXT,
	product_name TEXT,
	finished_product_name TEXT,
	material_type TEXT,
	board_width INT,
	board_height INT,
	board_thickness INT,
	num INT,
	texture_direction TEXT,
	edge_weight TEXT,
	dissimilar_shapes TEXT,
	rotate INT,
	front_slot TEXT,
	is_open_slot TEXT,
	back_slot TEXT,
	front_hole TEXT,
	is_drill_hold TEXT,
	back_hole TEXT,
	client_information TEXT,
	franchies_store TEXT,
	slot_broaching_flags TEXT,
	ranges_drill_flags TEXT,
	drill_hole TEXT,
	orders_type TEXT,
	FOREIGN KEY (mlid) REFERENCES material_list (mlid) ON DELETE CASCADE ON UPDATE CASCADE
);

CREATE TABLE material_group (
	mgid CHAR (36) PRIMARY KEY,
	name TEXT NOT NULL,
	mlid CHAR (36) NOT NULL,
	FOREIGN KEY (mlid) REFERENCES material_list (mlid) ON DELETE CASCADE ON UPDATE CASCADE
);

CREATE TABLE cutting_solution (
	csid CHAR (36) PRIMARY KEY,
	name TEXT,
	total_target_area FLOAT,
	source_board_width INT,
	source_board_height INT,
	source_board_thickness INT,
	total_source_board_num INT,
	total_utilization_rate FLOAT,
	mgid CHAR (36) NOT NULL,
	FOREIGN KEY (mgid) REFERENCES material_group (mgid) ON DELETE CASCADE ON UPDATE CASCADE
);

CREATE TABLE source_board (
	sbid CHAR (36) PRIMARY KEY,
	csid CHAR (36) NOT NULL,
	width INT,
    height INT,
    thickness INT,
	idx INTEGER,
	num INTEGER,
	utilization_area FLOAT,
	utilization_rate FLOAT,
	dxf_file_name TEXT,
	FOREIGN KEY (csid) REFERENCES cutting_solution (csid) ON DELETE CASCADE ON UPDATE CASCADE
);

CREATE TABLE board_rect (
	brid CHAR (36) PRIMARY KEY,
	sbid CHAR (36) NOT NULL,
	name TEXT,
	barcode_no TEXT,
	idx INTEGER,
	lt_x FLOAT NOT NULL,
	lt_y FLOAT NOT NULL,
	rb_x FLOAT NOT NULL,
	rb_y FLOAT NOT NULL,
	nc_start_pt_x FLOAT,
	nc_start_pt_y FLOAT,
	FOREIGN KEY (sbid) REFERENCES source_board (sbid) ON DELETE CASCADE ON UPDATE CASCADE
);