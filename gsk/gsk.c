/* GSK.C
 * eduarcastrillo@gmail.com
 */
#include "descriptor_tables.h"
#include "string.h"
#include "gsk.h"

void init_gsk() {
	init_descriptor_tables();
        init_mem();
}
