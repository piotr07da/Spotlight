import { spotlightApi } from "../preload";

declare global{
	interface Window { spotlightApi: typeof spotlightApi }
}
