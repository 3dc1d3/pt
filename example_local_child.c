void
run(pt_t *pt)
{
	struct local {
		pt_t reader;
		u16 stamp;
	} *l;

	// need to assign l twice so it's initialised on begin and contin.
	l = (local*) pt->local;
	pt_begin(pt);
	l = pt->local = (local*) malloc(sizeof(local));

	pt_loop(pt, pt_status(pt) != PT_STATUS_FINISHED) {
		u32 result;
		pt_loop(pt, pt_status(&l->reader) != PT_STATUS_FINISHED) {
			psi_read(&l->reader, &spi, &result);
		}
		pt_reset(&l->reader);

		double psi = calc_psi(result);
		printf("%u, %f\n", l->stamp, psi);
		l->stamp++;
	}

	pt_end(pt);
	free(l);
}
