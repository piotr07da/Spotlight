import { samplesApi } from "../preload";

declare global{
	interface Window { samplesApi: typeof samplesApi }
}
